#include "model_builder.h"
#include "shader_flags.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include "vertex_array_manager.h"
#include "texture_manager.h"
#include "default_material.h"
#include "pbr_material.h"
#include <unordered_map>
#include <stack>

const unsigned get_import_flags()
{
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

uint32_t parse_mesh_flags(const aiMesh* mesh)
{
	uint32_t flags = 0;

	if (mesh->HasPositions())
	{
		flags |= USE_VERTEX_POSITION;
	}

	if (mesh->HasTextureCoords(0))
	{
		flags |= USE_VERTEX_COORD;
	}

	if (mesh->HasVertexColors(0))
	{
		flags |= USE_VERTEX_COLOR;
	}

	if (mesh->HasNormals())
	{
		flags |= USE_VERTEX_NORMAL;
	}

	if (mesh->HasTangentsAndBitangents())
	{
		flags |= USE_VERTEX_TANGENT;
		flags |= USE_VERTEX_BITANGENT;
	}

	if ((flags & USE_VERTEX_TANGENT) && 
		(flags & USE_VERTEX_BITANGENT) &&
		(flags & USE_VERTEX_NORMAL))
	{
		flags |= USE_VERTEX_TBN_MATRIX;
	}
	/*if (mesh->HasBones())
	{
		flags |= USE_VERTEX_BONES; //TODO
	}*/

	return flags;
}

uint32_t parse_mesh_size(uint32_t flags)
{
	uint32_t element_count = 0; 

	if (flags & USE_VERTEX_POSITION)
	{
		element_count += 3;
	}

	if (flags & USE_VERTEX_COORD)
	{
		element_count += 2;
	}

	if (flags & USE_VERTEX_COLOR)
	{
		element_count += 4;
	}

	if (flags & USE_VERTEX_NORMAL)
	{
		element_count += 3;
	}

	if (flags & USE_VERTEX_TANGENT)
	{
		element_count += 3;
	}

	if (flags & USE_VERTEX_BITANGENT)
	{
		element_count += 3;
	}

	/*if (flags & USE_VERTEX_BONES)
	{
		element_count += 4; //TODO
	}*/

	return element_count * sizeof(float);
}

BufferLayoutRef parse_buffer_layout(uint32_t flags)
{
	std::vector<BufferElement> buffer_elements;

	//POSITION
	if (flags & USE_VERTEX_POSITION)
	{
		buffer_elements.emplace_back(ShaderDataType::Float3, "a_position");
	}

	//COORD
	if (flags & USE_VERTEX_COORD)
	{
		buffer_elements.emplace_back(ShaderDataType::Float2, "a_coord");
	}

	//COLOR
	if (flags & USE_VERTEX_COLOR)
	{
		buffer_elements.emplace_back(ShaderDataType::Float4, "a_color");
	}

	//NORMAL
	if (flags & USE_VERTEX_NORMAL)
	{
		buffer_elements.emplace_back(ShaderDataType::Float3, "a_normal");
	}

	//TANGENT
	if (flags & USE_VERTEX_TANGENT)
	{
		buffer_elements.emplace_back(ShaderDataType::Float3, "a_tangent");
	}

	//BITANGENT
	if (flags & USE_VERTEX_BITANGENT)
	{
		buffer_elements.emplace_back(ShaderDataType::Float3, "a_bitangent");
	}

	return std::make_shared<BufferLayout>(buffer_elements);
}

std::vector<Mesh> parse_meshes(const aiScene* scene)
{
	std::vector<Mesh> result;
	result.reserve(static_cast<size_t>(scene->mNumMeshes));

	glm::vec3 min(9999), max(-9999);

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
		verts.reserve(static_cast<size_t>(parse_mesh_size(mesh_flags) / sizeof(float)));
		
		for (unsigned j = 0; j < assimp_mesh->mNumVertices; ++j)
		{
			//POSITION
			if (mesh_flags & USE_VERTEX_POSITION)
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
			if (mesh_flags & USE_VERTEX_COORD)
			{
				auto& c = assimp_mesh->mTextureCoords[0][j];
				verts.emplace_back(c.x);
				verts.emplace_back(c.y);
			}

			//COLOR
			if(mesh_flags & USE_VERTEX_COLOR)
			{
				auto& c = assimp_mesh->mColors[0][j];
				verts.emplace_back(c.r);
				verts.emplace_back(c.g);
				verts.emplace_back(c.b);
				verts.emplace_back(c.a);
			}

			//NORMAL
			if(mesh_flags & USE_VERTEX_NORMAL)
			{
				auto& n = assimp_mesh->mNormals[j];
				verts.emplace_back(n.x);
				verts.emplace_back(n.y);
				verts.emplace_back(n.z);
			}

			//TANGENT
			if(mesh_flags & USE_VERTEX_TANGENT)
			{
				auto& t = assimp_mesh->mTangents[j];
				verts.emplace_back(t.x);
				verts.emplace_back(t.y);
				verts.emplace_back(t.z);
			}

			//BITANGENT
			if (mesh_flags & USE_VERTEX_BITANGENT)
			{
				auto& b = assimp_mesh->mBitangents[j];
				verts.emplace_back(b.x);
				verts.emplace_back(b.y);
				verts.emplace_back(b.z);
			}
		}

		std::vector<uint16_t> indices; //TODO: sometimes uint32_t?

		for (unsigned j = 0; j < assimp_mesh->mNumFaces; ++j)
		{
			auto& face = assimp_mesh->mFaces[j];

			for (unsigned k = 0; k < face.mNumIndices; ++k)
			{
				indices.emplace_back(static_cast<uint16_t>(face.mIndices[k]));
			}
		}

		auto layout = parse_buffer_layout(mesh_flags);

		auto vao = VertexArrayManager::get_instance()->make(verts,
			indices, layout);

		mesh.vao = vao;
	}
	//TODO

	printf("MIN %f %f %f\n", min.x, min.y, min.z);
	printf("MAX %f %f %f\n", max.x, max.y, max.z);

	return result;
}

glm::mat4 parse_transform(const aiMatrix4x4* from) //TODO: legit?
{
	glm::mat4 to;

	to[0][0] = from->a1; 
	to[0][1] = from->b1;  
	to[0][2] = from->c1; 
	to[0][3] = from->d1;

	to[1][0] = from->a2;
	to[1][1] = from->b2;  
	to[1][2] = from->c2; 
	to[1][3] = from->d2;

	to[2][0] = from->a3; 
	to[2][1] = from->b3; 
	to[2][2] = from->c3; 
	to[2][3] = from->d3;

	to[3][0] = from->a4;
	to[3][1] = from->b4;  
	to[3][2] = from->c4; 
	to[3][3] = from->d4;

	return to;
}

Node parse_node(const aiNode* node)
{
	Node result;
	
	result.name = std::string(node->mName.data, 
		node->mName.length); //TODO: legit?

	result.mesh_ids.reserve(static_cast<size_t>(node->mNumMeshes));

	for (unsigned i = 0; i < node->mNumMeshes; ++i)
	{
		result.mesh_ids.emplace_back(node->mMeshes[i]);
	}

	result.transform = parse_transform(&node->mTransformation);

	return result;
}

void link_nodes(std::vector<Node>& node_list,
	std::unordered_map<uint32_t, const aiNode*>& index_to_node,
	std::unordered_map<const aiNode*, uint32_t>& node_to_index)
{
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

std::vector<Node> parse_nodes(const aiScene* scene)
{
	std::vector<Node> result;

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

int to_index(aiString str)
{
	if (str.data[0] == '*')
	{
		str.data[0] = ' ';
		std::string sss(str.data, str.length);
		return std::stoi(sss);
	}

	return -1;
}

std::shared_ptr<AbstractTexture> get_texture(const aiScene* scene, int index)
{
	if (index < 0)
	{
		return nullptr;
	}

	if (static_cast<unsigned>(index) >= scene->mNumTextures)
	{
		return nullptr;
	}

	auto assimp_texture = scene->mTextures[index];

	const char* texture_ptr = 
		reinterpret_cast<const char*>(assimp_texture->pcData);
	uint32_t texture_size = assimp_texture->mWidth;

	if (assimp_texture->mHeight > 0)
	{
		texture_size *= assimp_texture->mHeight;
	}

	auto texture_manager = TextureManager::get_instance();
	return texture_manager->make(texture_ptr, texture_size);
}

AbstractMaterialRef parse_default_material(const aiScene* scene,
	const aiMaterial* material)
{
	auto& mat = *material;

	std::shared_ptr<AbstractTexture> diffuse;
	std::shared_ptr<AbstractTexture> normalmap;

	//DIFFUSE
	{
		aiString path;
		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &path))
		{
			if (path.length > 0)
			{
				diffuse = get_texture(scene, to_index(path));
			}
		}
	}

	//NORMALMAP
	{
		aiString path;
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &path))
		{
			if (path.length > 0)
			{
				normalmap = get_texture(scene, to_index(path));
			}
		}
	}

	return std::make_shared<DefaultMaterial>(diffuse, normalmap);
}

AbstractMaterialRef parse_pbr_material(const aiScene* scene,
	const aiMaterial* material)
{
	auto& mat = *material;

	std::shared_ptr<AbstractTexture> albedo;
	std::shared_ptr<AbstractTexture> ao_metallic_roughness;
	std::shared_ptr<AbstractTexture> normalmap;

	//ALBEDO
	{
		aiString path;
		if (mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE,
			&path) != AI_SUCCESS)
		{
			return nullptr;
		}

		if (path.length == 0)
		{
			return nullptr;
		}

		albedo = get_texture(scene, to_index(path));

		if (!albedo)
		{
			return nullptr;
		}
	}

	//AO_METALLIC_ROUGHNESS
	{
		aiString path;
		if(mat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE,
			&path) != AI_SUCCESS)
		{
			return nullptr;
		}

		if (path.length == 0)
		{
			return nullptr;
		}

		ao_metallic_roughness = get_texture(scene, to_index(path));

		if (!ao_metallic_roughness)
		{
			return nullptr;
		}
	}

	//NORMALMAP
	{
		aiString path;
		if (mat.GetTexture(aiTextureType_NORMALS, 0, &path))
		{
			if (path.length > 0)
			{
				normalmap = get_texture(scene, to_index(path));
			}
		}

		if (!normalmap)
		{
			if (mat.GetTexture(aiTextureType_HEIGHT, 0, &path))
			{
				if (path.length > 0)
				{
					normalmap = get_texture(scene, to_index(path));
				}
			}
		}
	}

	return std::make_shared<PBRMaterial>(albedo, ao_metallic_roughness, normalmap);
}

AbstractMaterialRef parse_material(const aiScene* scene,
	const aiMaterial* material)
{
	AbstractMaterialRef result;

	result = parse_pbr_material(scene, material);

	if (!result)
	{
		result = parse_default_material(scene, material);
	}

	return result;
}

std::vector<AbstractMaterialRef> parse_materials(const aiScene* scene)
{
	std::vector<AbstractMaterialRef> result;
	result.reserve(static_cast<size_t>(scene->mNumMaterials));

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		result.emplace_back(parse_material(scene, scene->mMaterials[i]));
	}

	return result;
}


ModelRef ModelBuilder::build(const char* filename)
{
	try
	{
		Assimp::Importer importer;	
		importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, std::numeric_limits<unsigned short>::max());
		importer.SetPropertyInteger(AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBBC_MAX_BONES, 35);
		//importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
		auto scene = importer.ReadFile(filename, get_import_flags());

		if (!scene)
		{
			return nullptr;
		}

		auto model = std::make_shared<Model>();
		model->meshes = std::move(parse_meshes(scene));
		model->nodes = std::move(parse_nodes(scene));
		model->materials = std::move(parse_materials(scene));
		//TODO

		return model;
	}
	catch (const std::exception& e)
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
