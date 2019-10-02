#include "model_builder.h"
#include "shader_flags.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

		//TODO
	}
	//TODO

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
	//TODO

	return model;
}
