#include "model_builder.h"
#include "shader_flags.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "vertex_array_manager.h"
#include <unordered_map>
#include <stack>

const unsigned get_import_flags()
{
	return aiProcessPreset_TargetRealtime_Quality |
		aiProcess_GenNormals |
		aiProcess_GenSmoothNormals |
		aiProcess_FindInstances |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SplitByBoneCount |
		aiProcess_SortByPType;
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
		buffer_elements.emplace_back(ShaderDataType::Float3, "a_bitangnet");
	}

	return std::make_shared<BufferLayout>(buffer_elements);
}

std::vector<Mesh> parse_meshes(const aiScene* scene)
{
	std::vector<Mesh> result;
	result.reserve(static_cast<size_t>(scene->mNumMeshes));

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
				indices.emplace_back(face.mIndices[k]);
			}
		}

		auto layout = parse_buffer_layout(mesh_flags);

		auto vao = VertexArrayManager::get_instance()->make(verts,
			indices, layout);

		mesh.vao = vao;
	}
	//TODO

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


ModelRef ModelBuilder::build(const char* filename)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(filename, get_import_flags());
	
	if (!scene)
	{
		return nullptr;
	}

	auto model = std::make_shared<Model>();
	model->meshes = std::move(parse_meshes(scene));
	model->nodes = std::move(parse_nodes(scene));
	//TODO

	return model;
}