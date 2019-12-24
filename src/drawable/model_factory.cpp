#include "model_factory.h"
#include "../shader/shader_flags.h"
#include "../image/image2d.h"
#include "../util/thread_pool.h"
#include "../util/log.h"
#include <assimp/material.h>
#include <unordered_map>
#include <optick.h>
#include <stack>

unsigned Rendy::ModelFactory::get_import_flags() const 
{
	OPTICK_EVENT();
	return //aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_GenNormals | //MAY NOT BE SPECIFIED TOGETHER WITH GenSmoothNormals
		aiProcess_GenSmoothNormals |
		aiProcess_FindInstances |
		aiProcess_FindDegenerates |
		aiProcess_FindInvalidData |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_LimitBoneWeights |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_SplitLargeMeshes |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenBoundingBoxes | //TODO
		aiProcess_SplitByBoneCount |
		aiProcess_SortByPType |
		aiProcess_FlipUVs;
}

uint32_t Rendy::ModelFactory::parse_mesh_flags(const aiMesh* mesh) const
{
	OPTICK_EVENT();

	uint32_t flags = 0;

	if (mesh->HasPositions())
	{
		flags |= Rendy::USE_VERTEX_POSITION;
		Log::info("USE_VERTEX_POSITION");
	}

	if (mesh->HasTextureCoords(0))
	{
		flags |= Rendy::USE_VERTEX_COORD;
		Log::info("USE_VERTEX_COORD");
	}

	if (mesh->HasVertexColors(0))
	{
		flags |= Rendy::USE_VERTEX_COLOR;
		Log::info("USE_VERTEX_COLOR");
	}

	if (mesh->HasNormals())
	{
		flags |= Rendy::USE_VERTEX_NORMAL;
		Log::info("USE_VERTEX_NORMAL");
	}

	if (mesh->HasTangentsAndBitangents())
	{
		flags |= Rendy::USE_VERTEX_TANGENT;
		flags |= Rendy::USE_VERTEX_BITANGENT;
		Log::info("USE_VERTEX_TANGENT");
		Log::info("USE_VERTEX_BITANGENT");
	}

	/*if ((flags & USE_VERTEX_TANGENT) && 
		(flags & USE_VERTEX_BITANGENT) &&
		(flags & USE_VERTEX_NORMAL))
	{
		flags |= USE_VERTEX_TBN_MATRIX;
		printf("USE_VERTEX_TBN_MATRIX\n");
	}*/
	
	if (mesh->HasBones())
	{
		flags |= USE_VERTEX_BONES;
		Log::info("USE_VERTEX_BONES");
	}

	return flags;
}

std::vector<Rendy::Bone> Rendy::ModelFactory::parse_mesh_bones(const aiMesh* mesh) const
{
	OPTICK_EVENT();

	std::vector<Bone> bones;
	bones.reserve(static_cast<size_t>(mesh->mNumBones));

	for (uint32_t i = 0; i < mesh->mNumBones; ++i)
	{
		auto assimp_bone = mesh->mBones[i];
		Bone bone;
		bone.name = std::string(assimp_bone->mName.data,
			assimp_bone->mName.length);
		bone.offset_matrix = parse_transform(assimp_bone->mOffsetMatrix);
		bones.emplace_back(std::move(bone));
	}

	return std::move(bones); //TODO?
}

std::unordered_map<std::string, std::vector<Rendy::Bone>> 
	Rendy::ModelFactory::parse_bones(const aiScene* scene) const
{
	OPTICK_EVENT();

	std::unordered_map<std::string, std::vector<Bone>> bone_map;

	for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		auto assimp_mesh = scene->mMeshes[i];
		std::string mesh_name(assimp_mesh->mName.data, 
			assimp_mesh->mName.length);

		bone_map[mesh_name] = std::move(parse_mesh_bones(assimp_mesh));
	}

	return std::move(bone_map); //TODO?
}

uint32_t Rendy::ModelFactory::parse_mesh_size(uint32_t flags) const
{
	OPTICK_EVENT();

	uint32_t element_count = 0; 

	if (flags & Rendy::USE_VERTEX_POSITION)
	{
		element_count += 3;
	}

	if (flags & Rendy::USE_VERTEX_COORD)
	{
		element_count += 2;
	}

	if (flags & Rendy::USE_VERTEX_COLOR)
	{
		element_count += 4;
	}

	if (flags & Rendy::USE_VERTEX_NORMAL)
	{
		element_count += 3;
	}

	if (flags & Rendy::USE_VERTEX_TANGENT)
	{
		element_count += 3;
	}

	if (flags & Rendy::USE_VERTEX_BITANGENT)
	{
		element_count += 3;
	}

	if (flags & Rendy::USE_VERTEX_BONES)
	{
		element_count += 8;
	}

	return element_count * sizeof(float);
}

Rendy::BufferLayoutRef Rendy::ModelFactory::parse_buffer_layout(uint32_t flags) const
{
	OPTICK_EVENT();

	std::vector<Rendy::BufferElement> buffer_elements;

	//POSITION
	if (flags & Rendy::USE_VERTEX_POSITION)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float3, "a_position");
	}

	//COORD
	if (flags & Rendy::USE_VERTEX_COORD)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float2, "a_coord");
	}

	//COLOR
	if (flags & Rendy::USE_VERTEX_COLOR)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float4, "a_color");
	}

	//NORMAL
	if (flags & Rendy::USE_VERTEX_NORMAL)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float3, "a_normal");
	}

	//TANGENT
	if (flags & Rendy::USE_VERTEX_TANGENT)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float3, "a_tangent");
	}

	//BITANGENT
	if (flags & Rendy::USE_VERTEX_BITANGENT)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float3, "a_bitangent");
	}

	//BONES
	if (flags & Rendy::USE_VERTEX_BONES)
	{
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float4, "a_bone_id");
		buffer_elements.emplace_back(Rendy::ShaderDataType::Float4, "a_weight");
	}
	
	return std::make_shared<Rendy::BufferLayout>(buffer_elements);
}

template<class T>
static Rendy::AbstractBufferRef parse_ibo(aiMesh* assimp_mesh, Rendy::EngineRef engine)
{
	OPTICK_EVENT();

	std::vector<T> indices; //TODO: sometimes uint32_t?

	for (unsigned j = 0; j < assimp_mesh->mNumFaces; ++j)
	{
		auto& face = assimp_mesh->mFaces[j];

		for (unsigned k = 0; k < face.mNumIndices; ++k)
		{
			indices.emplace_back(static_cast<T>(face.mIndices[k]));
		}
	}

	return engine->make_ibo(static_cast<uint32_t>(indices.size() * sizeof(T)), indices.data());
}

std::vector<Rendy::Mesh> Rendy::ModelFactory::parse_meshes(const aiScene* scene)
{
	OPTICK_EVENT();

	std::vector<Rendy::Mesh> result;
	result.reserve(static_cast<size_t>(scene->mNumMeshes));

	glm::vec3 min(9999), max(-9999);

	glm::vec3 mmin(9999), mmax(-9999);

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		result.emplace_back();
		auto& mesh = result[result.size() - 1];

		auto assimp_mesh = scene->mMeshes[i];
		auto mesh_flags = parse_mesh_flags(assimp_mesh);

		mesh.flags = mesh_flags;
		mesh.material_id = assimp_mesh->mMaterialIndex;
		mesh.name = std::string(assimp_mesh->mName.data, assimp_mesh->mName.length); //TODO: legit?

		std::vector<float> verts;
		verts.reserve(static_cast<size_t>(parse_mesh_size(mesh_flags) / sizeof(float))); //TODO
		

		mmin = glm::min(mmin, glm::vec3
			{
				assimp_mesh->mAABB.mMin.x,
				assimp_mesh->mAABB.mMin.y,
				assimp_mesh->mAABB.mMin.z
			});

		mmax = glm::max(mmax, glm::vec3
			{
				assimp_mesh->mAABB.mMax.x,
				assimp_mesh->mAABB.mMax.y,
				assimp_mesh->mAABB.mMax.z
			});

		uint32_t bones_offset = 0;

		std::unordered_map<uint32_t, std::vector<aiVertexWeight>> weight_map;

		if (mesh_flags & Rendy::USE_VERTEX_BONES)
		{
			weight_map.reserve(static_cast<size_t>(assimp_mesh->mNumBones));

			for (uint32_t j = 0; j < assimp_mesh->mNumBones; j++)
			{
				const aiBone* pBone = assimp_mesh->mBones[j];
				weight_map[j].reserve(static_cast<size_t>(pBone->mNumWeights));

				for (uint32_t k = 0; k < pBone->mNumWeights; k++)
				{
					weight_map[pBone->mWeights[k].mVertexId].
						emplace_back(j, pBone->mWeights[k].mWeight);
				}
			}
		}

		for (unsigned j = 0; j < assimp_mesh->mNumVertices; ++j)
		{
			//POSITION
			if (mesh_flags & Rendy::USE_VERTEX_POSITION)
			{
				auto& v = assimp_mesh->mVertices[j];
				verts.emplace_back(v.x);
				verts.emplace_back(v.y);
				verts.emplace_back(v.z);

				bones_offset += 3;

				for (int l = 0; l < 3; ++l)
				{
					min[l] = glm::min(min[l], v[l]);
					max[l] = glm::max(max[l], v[l]);
				}
			}

			//COORD
			if (mesh_flags & Rendy::USE_VERTEX_COORD)
			{
				auto& c = assimp_mesh->mTextureCoords[0][j];
				verts.emplace_back(c.x);
				verts.emplace_back(c.y);

				bones_offset += 2;
			}

			//COLOR
			if(mesh_flags & Rendy::USE_VERTEX_COLOR)
			{
				auto& c = assimp_mesh->mColors[0][j];
				verts.emplace_back(c.r);
				verts.emplace_back(c.g);
				verts.emplace_back(c.b);
				verts.emplace_back(c.a);

				bones_offset += 4;
			}

			//NORMAL
			if(mesh_flags & Rendy::USE_VERTEX_NORMAL)
			{
				auto& n = assimp_mesh->mNormals[j];
				verts.emplace_back(n.x);
				verts.emplace_back(n.y);
				verts.emplace_back(n.z);

				bones_offset += 3;
			}

			//TANGENT
			if(mesh_flags & Rendy::USE_VERTEX_TANGENT)
			{
				auto& t = assimp_mesh->mTangents[j];
				verts.emplace_back(t.x);
				verts.emplace_back(t.y);
				verts.emplace_back(t.z);

				bones_offset += 3;
			}

			//BITANGENT
			if (mesh_flags & Rendy::USE_VERTEX_BITANGENT)
			{
				auto& b = assimp_mesh->mBitangents[j];
				verts.emplace_back(b.x);
				verts.emplace_back(b.y);
				verts.emplace_back(b.z);

				bones_offset += 3;
			}

			//BONE
			if (mesh_flags & Rendy::USE_VERTEX_BONES)
			{
				assert(weight_map[j].size() <= 4);

				for (uint32_t k = 0; k < static_cast<uint32_t>(weight_map[j].size()); ++k)
				{
					verts.emplace_back(static_cast<float>(weight_map[j][k].mVertexId));
				}
				for (uint32_t k = static_cast<uint32_t>(weight_map[j].size()); k < 4; ++k)
				{
					verts.emplace_back(0.0f);
				}
				for (uint32_t k = 0; k < static_cast<uint32_t>(weight_map[j].size()); ++k)
				{
					verts.emplace_back(weight_map[j][k].mWeight);
				}
				for (uint32_t k = static_cast<uint32_t>(weight_map[j].size()); k < 4; ++k)
				{
					verts.emplace_back(0.0f);
				}
			}
		}

		auto layout = parse_buffer_layout(mesh_flags);

		auto vbo = engine->make_vbo(static_cast<uint32_t>(verts.size() * sizeof(float)), verts.data());
		AbstractBufferRef ibo;
		
		if (engine->get_index_type() == IndexType::UnsignedShort)
		{
			ibo = parse_ibo<uint16_t>(assimp_mesh, engine);
		}
		else
		{
			ibo = parse_ibo<uint32_t>(assimp_mesh, engine);
		}

		auto vao = engine->make_vao(vbo, ibo, layout);

		mesh.vao = vao;
	}
	//TODO

	Log::info("MIN ({0};{1};{2})", min.x, min.y, min.z);
	Log::info("MAX ({0};{1};{2})", max.x, max.y, max.z);

	Log::info("MMIN ({0};{1};{2})", mmin.x, mmin.y, mmin.z);
	Log::info("MMAX ({0};{1};{2})", mmax.x, mmax.y, mmax.z);

	uint32_t size = 0;

	for (auto& mesh : result)
	{
		size += mesh.vao->get_index_buffer()->get_size();
		size += mesh.vao->get_vertex_buffer()->get_size();
	}

	OPTICK_TAG("total size", size);

	return result;
}

static glm::vec3 to_vec3(const aiVector3D& v)
{
	OPTICK_EVENT();

	glm::vec3 result;

	result.x = v.x;
	result.y = v.y;
	result.z = v.z;

	return std::move(result); //TODO?
}

static glm::quat to_quat(const aiQuaternion& q)
{
	OPTICK_EVENT();

	glm::quat result;

	result.x = q.x;
	result.y = q.y;
	result.z = q.z;
	result.w = q.w;
	
	return std::move(result); //TODO?
}

Rendy::AnimationNodeRef Rendy::ModelFactory::parse_animation_channel
	(const aiNodeAnim* assimp_channel) const
{
	OPTICK_EVENT();

	AnimationNodeRef channel = std::make_shared<AnimationNode>();

	channel->name = std::string(assimp_channel->mNodeName.data,
		assimp_channel->mNodeName.length); //TODO: legit?
	channel->position_keys.resize(static_cast<size_t>(assimp_channel->mNumPositionKeys));
	channel->rotation_keys.resize(static_cast<size_t>(assimp_channel->mNumRotationKeys));
	channel->scaling_keys.resize(static_cast<size_t>(assimp_channel->mNumScalingKeys));

	for (uint32_t i = 0; i < assimp_channel->mNumPositionKeys; ++i)
	{
		auto& assimp_key = assimp_channel->mPositionKeys[i];
		auto& key = channel->position_keys[i];

		key.time = static_cast<float>(assimp_key.mTime);
		key.value = to_vec3(assimp_key.mValue);
	}

	for (uint32_t i = 0; i < assimp_channel->mNumRotationKeys; ++i)
	{
		auto& assimp_key = assimp_channel->mRotationKeys[i];
		auto& key = channel->rotation_keys[i];

		key.time = static_cast<float>(assimp_key.mTime);
		key.value = to_quat(assimp_key.mValue);
	}

	for (uint32_t i = 0; i < assimp_channel->mNumScalingKeys; ++i)
	{
		auto& assimp_key = assimp_channel->mScalingKeys[i];
		auto& key = channel->scaling_keys[i];

		key.time = static_cast<float>(assimp_key.mTime);
		key.value = to_vec3(assimp_key.mValue);
	}

	return channel;
}

std::vector<Rendy::AnimationNodeRef> Rendy::ModelFactory::parse_animation_channels
	(aiNodeAnim** assimp_channels, uint32_t channel_count) const
{
	OPTICK_EVENT();

	std::vector<AnimationNodeRef> channels;
	channels.reserve(static_cast<size_t>(channel_count));

	for (uint32_t i = 0; i < channel_count; ++i)
	{
		channels.emplace_back(parse_animation_channel(assimp_channels[i]));
	}

	return std::move(channels); //TODO?
}

Rendy::Animation Rendy::ModelFactory::parse_animation
	(const aiAnimation* assimp_animation) const
{
	OPTICK_EVENT();

	Animation result;

	Log::info("DURATION {0} (Assimp)", assimp_animation->mDuration);
	Log::info("TICKS PER SECOND {0} (Assimp)", assimp_animation->mTicksPerSecond);

	result.duration = static_cast<float>(assimp_animation->mDuration);
	result.name = std::string(assimp_animation->mName.data,
		assimp_animation->mName.length); //TODO: legit?
	result.ticks_per_second = static_cast<float>(assimp_animation->mTicksPerSecond);
	result.ticks_per_second = result.ticks_per_second != 0.0f ? 
		result.ticks_per_second * 24.0f : 25.0f; //TODO
	//result.ticks_per_second = 750.0f;
	Log::info("DURATION {0}", result.duration);
	Log::info("TICKS PER SECOND {0}", result.ticks_per_second);

	result.channels = parse_animation_channels(assimp_animation->mChannels, 
		assimp_animation->mNumChannels);

	return std::move(result); //TODO?
}

std::vector<Rendy::Animation> Rendy::ModelFactory::parse_animations
	(const aiScene* scene) const
{
	OPTICK_EVENT();

	std::vector<Animation> animations;
	animations.reserve(static_cast<size_t>(scene->mNumAnimations));

	for (uint32_t i = 0; i < scene->mNumAnimations; ++i)
	{
		animations.emplace_back(parse_animation(
			std::move(scene->mAnimations[i]))); //TODO: remove move?
	}

	return std::move(animations);
}

glm::mat4 Rendy::ModelFactory::parse_transform(const aiMatrix4x4& from) const
{
	OPTICK_EVENT();
	return glm::transpose(glm::make_mat4(&from.a1));
}

Rendy::Node Rendy::ModelFactory::parse_node(const aiNode* node)
{
	OPTICK_EVENT();
	OPTICK_TAG("mesh count", node->mNumMeshes);

	Rendy::Node result;
	
	result.name = std::string(node->mName.data, 
		node->mName.length); //TODO: legit?

	result.mesh_ids.reserve(static_cast<size_t>(node->mNumMeshes));

	for (unsigned i = 0; i < node->mNumMeshes; ++i)
	{
		result.mesh_ids.emplace_back(node->mMeshes[i]);
	}

	result.transform = parse_transform(node->mTransformation);

	return result;
}

void Rendy::ModelFactory::link_nodes(std::vector<Rendy::Node>& node_list,
	std::unordered_map<uint32_t, const aiNode*>& index_to_node,
	std::unordered_map<const aiNode*, uint32_t>& node_to_index)
{
	OPTICK_EVENT();
	for (uint32_t index = 0; index < static_cast<uint32_t>(node_list.size()); 
		++index)
	{
		auto& node = node_list[index];
		auto assimp_node = index_to_node[index];

		if (assimp_node->mParent == nullptr)
		{
			node.root_id = -1;
		}
		else
		{
			node.root_id = node_to_index[assimp_node->mParent];
		}

		node.child_ids.reserve(static_cast<size_t>(assimp_node->mNumChildren));

		for (unsigned j = 0; j < assimp_node->mNumChildren; ++j)
		{
			node.child_ids.emplace_back(node_to_index[assimp_node->mChildren[j]]);
		}
	}
}

std::vector<Rendy::Node> Rendy::ModelFactory::parse_nodes(const aiScene* scene)
{
	OPTICK_EVENT();

	std::vector<Rendy::Node> result;

	std::stack<const aiNode*> unhandled_nodes;
	unhandled_nodes.emplace(scene->mRootNode);

	std::unordered_map<const aiNode*, uint32_t> node_to_index;
	std::unordered_map<uint32_t, const aiNode*> index_to_node;

	while (!unhandled_nodes.empty())
	{
		auto node = unhandled_nodes.top();
		unhandled_nodes.pop();

		if (node_to_index.find(node) == node_to_index.end())
		{
			result.emplace_back(parse_node(node));

			const uint32_t index = static_cast<uint32_t>(result.size() - 1);
			node_to_index[node] = index;
			index_to_node[index] = node;

			for (unsigned i = 0; i < node->mNumChildren; ++i)
			{
				unhandled_nodes.emplace(node->mChildren[i]);
			}
		}
	}

	link_nodes(result, index_to_node, node_to_index);

	return result;
}

int Rendy::ModelFactory::to_index(aiString str, const aiScene* scene, 
	const std::unordered_map<const aiTexture*, int>& aitex_to_index) const
{
	OPTICK_EVENT();

	auto tex = scene->GetEmbeddedTexture(str.data);

	auto it = aitex_to_index.find(tex);
	if (it == aitex_to_index.end())
	{
		return -1;
	}

	return it->second;
}

Rendy::AbstractMaterialRef Rendy::ModelFactory::parse_material(const aiScene* scene,
	const aiMaterial* material, std::vector<Rendy::Image2DRef>& images,
	const std::unordered_map<const aiTexture*, int>& aitex_to_index)
{
	OPTICK_EVENT();

	auto image_set = form_image_set(scene, material, images, aitex_to_index);

	return engine->make_material(image_set);
}

std::vector<Rendy::AbstractMaterialRef> Rendy::ModelFactory::parse_materials
	(const aiScene* scene, std::vector<Rendy::Image2DRef>& images)
{
	OPTICK_EVENT();
	OPTICK_TAG("material count", scene->mNumMaterials);

	std::vector<Rendy::AbstractMaterialRef> result;
	result.reserve(static_cast<size_t>(scene->mNumMaterials));

	auto aitex_to_index = generate_image_map(scene);

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		result.emplace_back(parse_material(scene, scene->mMaterials[i],
			images, aitex_to_index));
	}

	return result;
}

std::unordered_map<const aiTexture*, int> 
Rendy::ModelFactory::generate_image_map(const aiScene* scene)
{
	OPTICK_EVENT();

	std::unordered_map<const aiTexture*, int> map;

	for (uint32_t i = 0; i < scene->mNumTextures; ++i)
	{
		map[scene->mTextures[i]] = i;
	}

	return map;
}

Rendy::Image2DRef Rendy::ModelFactory::parse_image(const aiTexture* assimp_texture)
{
	OPTICK_EVENT();

	const char* texture_ptr =
		reinterpret_cast<const char*>(assimp_texture->pcData);
	uint32_t texture_size = assimp_texture->mWidth;

	if (assimp_texture->mHeight > 0)
	{
		texture_size *= assimp_texture->mHeight;
	}

	OPTICK_TAG("texture_size", texture_size);

	return std::make_shared<Rendy::Image2D>(texture_ptr, texture_size);
}

std::vector<Rendy::Image2DRef> Rendy::ModelFactory::parse_images(const aiScene* scene)
{
	OPTICK_EVENT();
	OPTICK_TAG("image count", scene->mNumTextures);

	std::vector<Rendy::Image2DRef> result;
	result.reserve(static_cast<size_t>(scene->mNumTextures));

	{
		using Image2DFuture = std::shared_future<Rendy::Image2DRef>;
		std::vector<Image2DFuture> future_images;
		future_images.reserve(static_cast<size_t>(scene->mNumTextures));

		for (uint32_t i = 0; i < scene->mNumTextures; ++i)
		{
			OPTICK_EVENT("enqueue parse_image task")
			auto assimp_texture = scene->mTextures[i];
			OPTICK_TAG("width", assimp_texture->mWidth);
			OPTICK_TAG("height", assimp_texture->mHeight);
			OPTICK_TAG("format hint", assimp_texture->achFormatHint);

			using namespace std::placeholders;
			Image2DFuture future = thread_pool->enqueue(std::bind(&ModelFactory::parse_image, this, _1),
				assimp_texture);
			future_images.emplace_back(future);
		}

		for (auto& future_image : future_images)
		{
			result.emplace_back(future_image.get());
		}
	}

	return std::move(result);
}

Rendy::ImageSetRef Rendy::ModelFactory::form_image_set(const aiScene* scene, 
	const aiMaterial* material, std::vector<Image2DRef>& images,
	const std::unordered_map<const aiTexture*, int>& aitex_to_index) const
{
	OPTICK_EVENT();

	auto image_set = std::make_shared<ImageSet>();

	auto& mat = *material;

	//DIFFUSE
	{
		aiString path;
		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			if (path.length > 0)
			{
				image_set->color = get_image(to_index(path, scene, 
					aitex_to_index), images);
			}
		}

		if (!image_set->color)
		{
			if (mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, 
				&path) == AI_SUCCESS)
			{
				if (path.length > 0)
				{
					image_set->color = get_image(to_index(path, scene, 
						aitex_to_index), images);
				}
			}
		}
	}

	//NORMALMAP
	{
		aiString path;
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
		{
			if (path.length > 0)
			{
				image_set->normal = get_image(to_index(path, scene, 
					aitex_to_index), images);
			}
		}

		if (!image_set->normal)
		{
			if (mat.GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
			{
				if (path.length > 0)
				{
					image_set->normal = get_image(to_index(path, scene, 
						aitex_to_index), images);
				}
			}
		}
	}

	//AO_METALLIC_ROUGHNESS
	{
		aiString path;
		if (mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE,
			&path) == AI_SUCCESS)
		{
			if (path.length > 0)
			{
				image_set->metallic_roughness = get_image(to_index(path, scene, 
					aitex_to_index), images);
			}
		}
	}

	return image_set;
}

Rendy::Image2DRef Rendy::ModelFactory::get_image(int index, std::vector<Image2DRef>& images) const
{
	OPTICK_EVENT();

	if (static_cast<size_t>(index) >= images.size() || index < 0)
	{
		return nullptr;
	}

	return images[index];
}

Rendy::ModelFactory::ModelFactory(EngineRef engine, VFSRef vfs, ThreadPoolRef thread_pool)
{
	OPTICK_EVENT();
	this->engine = engine;
	this->vfs = vfs;
	this->thread_pool = thread_pool;
}

Rendy::ModelRef Rendy::ModelFactory::make(const char* filename)
{
	OPTICK_EVENT();
	OPTICK_TAG("filename", filename);

	OPTICK_PUSH("read file");
	auto file = vfs->open_file(filename, Rendy::FileMode::Read);
	
	if (!file->validate())
	{
		return nullptr;
	}

	std::vector<uint8_t> memory;
	memory.resize(static_cast<size_t>(file->get_size()));
	file->read(memory.data(), file->get_size());

	Log::info("file size {0}", file->get_size());
	OPTICK_POP();

	return make(memory.data(), static_cast<uint32_t>(memory.size()));
}

Rendy::ModelRef Rendy::ModelFactory::make(const void* memory, uint32_t size)
{
	OPTICK_EVENT();

	try
	{
		Assimp::Importer importer;

		if (engine->get_index_type() == IndexType::UnsignedShort)
		{
			importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 
				std::numeric_limits<unsigned short>::max());
		}
		else
		{
			importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 
				AI_SLM_DEFAULT_MAX_VERTICES);
		}

		importer.SetPropertyInteger(AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBBC_MAX_BONES, 120);
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		const aiScene* scene = nullptr;

		{
			OPTICK_PUSH("importer.ReadFileFromMemory (glb)");
			Log::info("Loading glb model...");
			scene = importer.ReadFileFromMemory(memory, static_cast<size_t>(size), get_import_flags(), "glb");
			OPTICK_POP();
		}

		if (!scene)
		{
			Log::warn("Can't load glb model, maybe it's a fbx one?");
			OPTICK_PUSH("importer.ReadFileFromMemory (fbx)");
			scene = importer.ReadFileFromMemory(memory, static_cast<size_t>(size), get_import_flags(), "fbx");
			OPTICK_POP();
		}

		if (!scene)
		{
			Log::error("Can't load model: {0}", importer.GetErrorString());
			return nullptr;
		}

		Log::info("Assimp model loaded");
		Log::info("Parsing assimp model...");

		Log::info("(BEFORE)");
		auto model = std::make_shared<Model>();
		Log::info("NODES");
		model->nodes = std::move(parse_nodes(scene));
		model->calculate_cache();
		Log::info("MESHES");
		model->meshes = std::move(parse_meshes(scene));
		Log::info("IMAGES");
		model->images = std::move(parse_images(scene));
		Log::info("MATERIALS");
		model->materials = std::move(parse_materials(scene, model->images));
		Log::info("BONES");
		model->name_to_bones = std::move(parse_bones(scene));
		Log::info("ANIMATIONS");
		model->animations = std::move(parse_animations(scene));
		Log::info("Animation count: {0}", scene->mNumAnimations); //TODO
		Log::info("DONE");

		return model;
	}
	catch (const std::exception & e)
	{
		//TODO: error
		return nullptr;
	}
	catch (...)
	{
		//TODO: error
		return nullptr;
	}
}
