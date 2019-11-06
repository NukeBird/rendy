#include "model_factory.h"
#include "shader_flags.h"
#include "image2d.h"
#include "thread_pool.h"
#include <unordered_map>
#include <stack>

#include <optick.h>

unsigned Rendy::ModelFactory::get_import_flags() const 
{
	OPTICK_EVENT();
	return //aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_GenNormals |
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
		printf("USE_VERTEX_POSITION\n");
	}

	if (mesh->HasTextureCoords(0))
	{
		flags |= Rendy::USE_VERTEX_COORD;
		printf("USE_VERTEX_COORD\n");
	}

	if (mesh->HasVertexColors(0))
	{
		flags |= Rendy::USE_VERTEX_COLOR;
		printf("USE_VERTEX_COLOR\n");
	}

	if (mesh->HasNormals())
	{
		flags |= Rendy::USE_VERTEX_NORMAL;
		printf("USE_VERTEX_NORMAL\n");
	}

	if (mesh->HasTangentsAndBitangents())
	{
		flags |= Rendy::USE_VERTEX_TANGENT;
		flags |= Rendy::USE_VERTEX_BITANGENT;
		printf("USE_VERTEX_TANGENT\n");
		printf("USE_VERTEX_BITANGENT\n");
	}

	/*if ((flags & USE_VERTEX_TANGENT) && 
		(flags & USE_VERTEX_BITANGENT) &&
		(flags & USE_VERTEX_NORMAL))
	{
		flags |= USE_VERTEX_TBN_MATRIX;
		printf("USE_VERTEX_TBN_MATRIX\n");
	}*/
	/*if (mesh->HasBones())
	{
		flags |= USE_VERTEX_BONES; //TODO
	}*/

	printf("\n");

	return flags;
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

	/*if (flags & USE_VERTEX_BONES)
	{
		element_count += 4; //TODO
	}*/

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

	return std::make_shared<Rendy::BufferLayout>(buffer_elements);
}

template<class T>
static Rendy::AbstractBufferRef parse_ibo(aiMesh* assimp_mesh, Rendy::AbstractEngineRef engine)
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

		for (unsigned j = 0; j < assimp_mesh->mNumVertices; ++j)
		{
			//POSITION
			if (mesh_flags & Rendy::USE_VERTEX_POSITION)
			{
				auto& v = assimp_mesh->mVertices[j];
				verts.emplace_back(v.x);
				verts.emplace_back(v.y);
				verts.emplace_back(v.z);

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
			}

			//COLOR
			if(mesh_flags & Rendy::USE_VERTEX_COLOR)
			{
				auto& c = assimp_mesh->mColors[0][j];
				verts.emplace_back(c.r);
				verts.emplace_back(c.g);
				verts.emplace_back(c.b);
				verts.emplace_back(c.a);
			}

			//NORMAL
			if(mesh_flags & Rendy::USE_VERTEX_NORMAL)
			{
				auto& n = assimp_mesh->mNormals[j];
				verts.emplace_back(n.x);
				verts.emplace_back(n.y);
				verts.emplace_back(n.z);
			}

			//TANGENT
			if(mesh_flags & Rendy::USE_VERTEX_TANGENT)
			{
				auto& t = assimp_mesh->mTangents[j];
				verts.emplace_back(t.x);
				verts.emplace_back(t.y);
				verts.emplace_back(t.z);
			}

			//BITANGENT
			if (mesh_flags & Rendy::USE_VERTEX_BITANGENT)
			{
				auto& b = assimp_mesh->mBitangents[j];
				verts.emplace_back(b.x);
				verts.emplace_back(b.y);
				verts.emplace_back(b.z);
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

	printf("MIN %f %f %f\n", min.x, min.y, min.z);
	printf("MAX %f %f %f\n", max.x, max.y, max.z);

	printf("MMIN %f %f %f\n", mmin.x, mmin.y, mmin.z);
	printf("MMAX %f %f %f\n", mmax.x, mmax.y, mmax.z);

	uint32_t size = 0;

	for (auto& mesh : result)
	{
		size += mesh.vao->get_index_buffer()->get_size();
		size += mesh.vao->get_vertex_buffer()->get_size();
	}

	OPTICK_TAG("total size", size);

	return result;
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
			node.root_id = 0;
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

std::vector<Rendy::AbstractMaterialRef> Rendy::ModelFactory::parse_materials(const aiScene* scene,
	std::vector<Rendy::Image2DRef>& images)
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

	float ior;
	mat.Get(AI_MATKEY_SHININESS, ior);

	printf("IOR %f\n", ior);

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

	if (index >= images.size() || index < 0)
	{
		return nullptr;
	}

	return images[index];
}

Rendy::ModelFactory::ModelFactory(AbstractEngineRef engine, VFSRef vfs, ThreadPoolRef thread_pool)
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

	printf("file size %d\n", file->get_size());
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
		importer.SetPropertyInteger(AI_CONFIG_PP_SBBC_MAX_BONES, 35);
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		const aiScene* scene = nullptr;

		{
			OPTICK_PUSH("importer.ReadFileFromMemory (glb)");
			scene = importer.ReadFileFromMemory(memory, static_cast<size_t>(size), get_import_flags(), "glb"); //TODO: pass fbx hint too
			OPTICK_POP();
		}

		if (!scene)
		{
			OPTICK_PUSH("importer.ReadFileFromMemory (fbx)");
			scene = importer.ReadFileFromMemory(memory, static_cast<size_t>(size), get_import_flags(), "fbx"); //TODO: pass fbx hint too
			OPTICK_POP();
		}

		if (!scene)
		{
			printf("%s\n", importer.GetErrorString());
			return nullptr;
		}

		printf("(BEFORE)\n");
		auto model = std::make_shared<Model>();
		printf("MESHES\n");
		model->meshes = std::move(parse_meshes(scene));
		printf("NODES\n");
		model->nodes = std::move(parse_nodes(scene));
		printf("IMAGES\n");
		model->images = std::move(parse_images(scene));
		printf("MATERIALS\n");
		model->materials = std::move(parse_materials(scene, model->images));
		printf("DONE\n");
		//TODO

		printf("Animation count: %d\n", scene->mNumAnimations); //TODO

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
