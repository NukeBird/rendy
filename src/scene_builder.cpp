#include "scene_builder.h"
#include "shader_flags.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include "vertex_array_factory.h"
#include "texture_factory.h"
#include "default_material.h"
#include "pbr_material.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <future>

static const unsigned get_import_flags()
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

static std::string to_string(const aiString& str)
{
	return std::string(str.data, str.length);
}

static glm::vec3 to_vec3(const aiVector3D& vec)
{
	return glm::vec3{ vec.x, vec.y, vec.z };
}

static glm::mat4 to_mat4(const aiMatrix4x4& from)
{
	return glm::transpose(glm::make_mat4(&from.a1));
}

static uint32_t parse_mesh_flags(const aiMesh* mesh)
{
	uint32_t flags = 0;

	if (mesh->HasPositions())
	{
		flags |= USE_VERTEX_POSITION;
		printf("USE_VERTEX_POSITION\n");
	}

	if (mesh->HasTextureCoords(0))
	{
		flags |= USE_VERTEX_COORD;
		printf("USE_VERTEX_COORD\n");
	}

	if (mesh->HasVertexColors(0))
	{
		flags |= USE_VERTEX_COLOR;
		printf("USE_VERTEX_COLOR\n");
	}

	if (mesh->HasNormals())
	{
		flags |= USE_VERTEX_NORMAL;
		printf("USE_VERTEX_NORMAL\n");
	}

	if (mesh->HasTangentsAndBitangents())
	{
		flags |= USE_VERTEX_TANGENT;
		flags |= USE_VERTEX_BITANGENT;
		printf("USE_VERTEX_TANGENT\n");
		printf("USE_VERTEX_BITANGENT\n");
	}

	if ((flags & USE_VERTEX_TANGENT) &&
		(flags & USE_VERTEX_BITANGENT) &&
		(flags & USE_VERTEX_NORMAL))
	{
		flags |= USE_VERTEX_TBN_MATRIX;
		printf("USE_VERTEX_TBN_MATRIX\n");
	}
	/*if (mesh->HasBones())
	{
		flags |= USE_VERTEX_BONES; //TODO
	}*/

	printf("\n");

	return flags;
}

static uint32_t parse_mesh_size(uint32_t flags)
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

static BufferLayoutRef parse_buffer_layout(uint32_t flags)
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

static std::vector<float> parse_verts(const aiMesh* assimp_mesh, uint32_t flags)
{
	std::vector<float> verts;
	verts.reserve(static_cast<size_t>(parse_mesh_size(flags) / sizeof(float)));

	for (unsigned j = 0; j < assimp_mesh->mNumVertices; ++j)
	{
		//POSITION
		if (flags & USE_VERTEX_POSITION)
		{
			auto& v = assimp_mesh->mVertices[j];
			verts.emplace_back(v.x);
			verts.emplace_back(v.y);
			verts.emplace_back(v.z);
		}

		//COORD
		if (flags & USE_VERTEX_COORD)
		{
			auto& c = assimp_mesh->mTextureCoords[0][j];
			verts.emplace_back(c.x);
			verts.emplace_back(c.y);
		}

		//COLOR
		if (flags & USE_VERTEX_COLOR)
		{
			auto& c = assimp_mesh->mColors[0][j];
			verts.emplace_back(c.r);
			verts.emplace_back(c.g);
			verts.emplace_back(c.b);
			verts.emplace_back(c.a);
		}

		//NORMAL
		if (flags & USE_VERTEX_NORMAL)
		{
			auto& n = assimp_mesh->mNormals[j];
			verts.emplace_back(n.x);
			verts.emplace_back(n.y);
			verts.emplace_back(n.z);
		}

		//TANGENT
		if (flags & USE_VERTEX_TANGENT)
		{
			auto& t = assimp_mesh->mTangents[j];
			verts.emplace_back(t.x);
			verts.emplace_back(t.y);
			verts.emplace_back(t.z);
		}

		//BITANGENT
		if (flags & USE_VERTEX_BITANGENT)
		{
			auto& b = assimp_mesh->mBitangents[j];
			verts.emplace_back(b.x);
			verts.emplace_back(b.y);
			verts.emplace_back(b.z);
		}
	}

	return std::move(verts);
}

static std::vector<uint16_t> parse_indices(const aiMesh* assimp_mesh)
{
	std::vector<uint16_t> indices;
	indices.reserve(assimp_mesh->mNumFaces * 3);

	for (unsigned j = 0; j < assimp_mesh->mNumFaces; ++j)
	{
		auto& face = assimp_mesh->mFaces[j];

		for (unsigned k = 0; k < face.mNumIndices; ++k)
		{
			indices.emplace_back(static_cast<uint16_t>(face.mIndices[k])); //TODO
		}
	}

	return std::move(indices);
}

static void parse_meshes(const aiScene* scene, SceneRef& result)
{
	result->mesh_list.resize(static_cast<size_t>(scene->mNumMeshes));

	using FutureGeometryVerts = std::shared_future<std::vector<float>>;
	std::vector<FutureGeometryVerts> vert_list;

	using FutureGeometryIndices = std::shared_future<std::vector<uint16_t>>;
	std::vector<FutureGeometryIndices> index_list;

	std::vector<BufferLayoutRef> layouts;

	vert_list.reserve(static_cast<size_t>(scene->mNumMeshes));
	index_list.reserve(static_cast<size_t>(scene->mNumMeshes));
	layouts.reserve(static_cast<size_t>(scene->mNumMeshes));

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		auto& mesh = result->mesh_list[result->mesh_list.size() - 1];

		auto assimp_mesh = scene->mMeshes[i];
		auto mesh_flags = parse_mesh_flags(assimp_mesh);

		mesh.name = to_string(assimp_mesh->mName);
		mesh.flags = mesh_flags;
		mesh.material_id = assimp_mesh->mMaterialIndex;

		auto future_verts = std::async(parse_verts, assimp_mesh, mesh_flags);
		vert_list.emplace_back(std::move(future_verts));

		auto future_indices = std::async(parse_indices, assimp_mesh);
		index_list.emplace_back(std::move(future_indices));

		layouts.emplace_back(parse_buffer_layout(mesh_flags));
	}

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		auto& mesh = result->mesh_list[i];

		auto vao = VertexArrayFactory::get_instance()->make
		(
			vert_list[i].get(),
			index_list[i].get(), 
			layouts[i]
		);

		mesh.vao = vao;
	}
}

static void parse_cameras(const aiScene* assimp_scene, SceneRef& result)
{
	result->camera_list.resize(static_cast<size_t>(assimp_scene->mNumCameras));

	for (uint32_t i = 0; i < assimp_scene->mNumCameras; ++i)
	{
		auto camera = assimp_scene->mCameras[i];
		auto& result_camera = result->camera_list[i];

		result_camera.name = to_string(camera->mName);
		result_camera.near = glm::max(camera->mClipPlaneNear, 0.005f);
		result_camera.far = glm::max(camera->mClipPlaneFar, result_camera.near + 0.05f);
		result_camera.fov = camera->mHorizontalFOV;
		result_camera.look_at = to_vec3(camera->mLookAt);
		result_camera.position = to_vec3(camera->mPosition);
		result_camera.up = to_vec3(camera->mUp);
	}
}

static AbstractTextureRef parse_texture(const aiTexture* assimp_texture)
{
	const char* texture_ptr =
		reinterpret_cast<const char*>(assimp_texture->pcData);
	uint32_t texture_size = assimp_texture->mWidth;

	if (assimp_texture->mHeight > 0)
	{
		texture_size *= assimp_texture->mHeight;
	}

	auto texture_manager = TextureFactory::get_instance();
	return texture_manager->make(texture_ptr, texture_size);
}

static void parse_textures(const aiScene* assimp_scene, SceneRef& result)
{
	result->texture_list.reserve(static_cast<size_t>(assimp_scene->mNumTextures));

	for (uint32_t i = 0; i < assimp_scene->mNumTextures; ++i)
	{
		auto assimp_texture = assimp_scene->mTextures[i];
		result->texture_list.emplace_back(parse_texture(assimp_texture));
	}
}

static NodeS::NodeRef parse_node(const aiNode* assimp_node)
{
	if (!assimp_node)
	{
		return nullptr;
	}

	NodeS::NodeRef node = std::make_shared<NodeS>();

	node->name = to_string(assimp_node->mName);
	node->transform = to_mat4(assimp_node->mTransformation);
	
	node->mesh_ids.reserve(assimp_node->mNumMeshes);
	for (uint32_t i = 0; i < assimp_node->mNumMeshes; ++i)
	{
		node->mesh_ids.emplace_back(assimp_node->mMeshes[i]);
	}

	node->child_list.reserve(assimp_node->mNumChildren);
	for (uint32_t i = 0; i < assimp_node->mNumChildren; ++i)
	{
		node->child_list.emplace_back(parse_node(assimp_node->mChildren[i]));
	}

	return node;
}

static void parse_nodes(const aiScene* assimp_scene, SceneRef& result)
{
	result->root_node = parse_node(assimp_scene->mRootNode);
}

static int to_index(aiString str)
{
	if (str.data[0] == '*')
	{
		str.data[0] = ' ';
		std::string sss(str.data, str.length);
		return std::stoi(sss);
	}

	return -1;
}

static AbstractTextureRef parse_texture(const aiMaterial* material,
	std::vector<AbstractTextureRef>& texture_list,
	const aiTextureType& type, uint32_t assimp_index = 0)
{
	AbstractTextureRef result;

	aiString path;
	if (material->GetTexture(type, assimp_index, &path))
	{
		if (path.length > 0)
		{
			auto index = to_index(path);

			if (index >= 0 && index < texture_list.size())
			{
				result = texture_list[index];
			}
		}
	}

	return result;
}

static AbstractMaterialRef parse_default_material(const aiMaterial* material,
	std::vector<AbstractTextureRef>& texture_list)
{
	auto& mat = *material;

	std::shared_ptr<AbstractTexture> diffuse;
	std::shared_ptr<AbstractTexture> normalmap;

	diffuse = parse_texture(material, texture_list,
		aiTextureType_DIFFUSE);

	normalmap = parse_texture(material, texture_list,
		aiTextureType_NORMALS);

	return std::make_shared<DefaultMaterial>(diffuse, normalmap);
}

static AbstractMaterialRef parse_pbr_material(const aiMaterial* material,
	std::vector<AbstractTextureRef>& texture_list)
{
	auto& mat = *material;

	std::shared_ptr<AbstractTexture> albedo;
	std::shared_ptr<AbstractTexture> ao_metallic_roughness;
	std::shared_ptr<AbstractTexture> normalmap;

	albedo = parse_texture(material, texture_list,
		AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE); 

	if (!albedo)
	{
		return nullptr;
	}
	
	ao_metallic_roughness = parse_texture(material, texture_list,
		AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE);

	if (!ao_metallic_roughness)
	{
		return nullptr;
	}

	normalmap = parse_texture(material, texture_list,
		aiTextureType_NORMALS);

	return std::make_shared<PBRMaterial>(albedo, ao_metallic_roughness, normalmap);
}

static AbstractMaterialRef parse_material(const aiMaterial* material, 
	std::vector<AbstractTextureRef>& texture_list)
{
	AbstractMaterialRef result;
	
	result = parse_pbr_material(material, texture_list);

	if (!result)
	{
		result = parse_default_material(material, texture_list);
	}

	return result;
}

static void parse_materials(const aiScene* assimp_scene, SceneRef& result)
{
	result->material_list.reserve(static_cast<size_t>(assimp_scene->mNumMaterials));

	for (unsigned i = 0; i < assimp_scene->mNumMaterials; ++i)
	{
		result->material_list.emplace_back(parse_material(
			assimp_scene->mMaterials[i], result->texture_list));
	}
}

static void parse_scene(const aiScene* assimp_scene, SceneRef& result)
{
	parse_meshes(assimp_scene, result);
	parse_textures(assimp_scene, result);
	parse_materials(assimp_scene, result);
	parse_nodes(assimp_scene, result);
	//TODO: parse_animations
	parse_cameras(assimp_scene, result);
}

SceneRef SceneBuilder::build(const char* filename)
{
	SceneRef result = std::make_shared<Scene>();
	try
	{
		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, std::numeric_limits<unsigned short>::max());
		importer.SetPropertyInteger(AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBBC_MAX_BONES, 35);
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		auto scene = importer.ReadFile(filename, get_import_flags());

		if (!scene)
		{
			printf("%s\n", importer.GetErrorString());
			return nullptr;
		}

		parse_scene(scene, result);
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

	return result;
}
