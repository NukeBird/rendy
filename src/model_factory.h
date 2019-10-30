#pragma once
#include "model.h"
#include "thread_pool.h"
#include "abstract_engine.h"
#include "abstract_vfs.h"
#include "image_set.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Rendy
{
	class ModelFactory
	{
	public:
		ModelFactory(AbstractEngineRef engine, VFSRef vfs, ThreadPoolRef thread_pool 
			= std::make_shared<ThreadPool>());
		ModelRef make(const char* filename);
		ModelRef make(const void* memory, uint32_t size);
	private:
		unsigned get_import_flags() const;
		uint32_t parse_mesh_flags(const aiMesh* mesh) const;
		uint32_t parse_mesh_size(uint32_t flags) const;
		BufferLayoutRef parse_buffer_layout(uint32_t flags) const;
		std::vector<Mesh> parse_meshes(const aiScene* scene);
		glm::mat4 parse_transform(const aiMatrix4x4& from) const;
		Node parse_node(const aiNode* node);
		void link_nodes(std::vector<Node>& node_list,
			std::unordered_map<uint32_t, const aiNode*>& index_to_node,
			std::unordered_map<const aiNode*, uint32_t>& node_to_index);
		std::vector<Node> parse_nodes(const aiScene* scene);
		int to_index(aiString str) const;
		AbstractMaterialRef parse_material(const aiScene* scene,
			const aiMaterial* material, std::vector<Image2DRef>& images);
		std::vector<AbstractMaterialRef> parse_materials(const aiScene* scene,
			std::vector<Image2DRef>& images);
		Image2DRef parse_image(const aiTexture* assimp_texture);
		std::vector<Image2DRef> parse_images(const aiScene* scene);
		ImageSetRef form_image_set(const aiScene* scene,
			const aiMaterial* material, std::vector<Image2DRef>& images) const;
		Image2DRef get_image(int index, std::vector<Image2DRef>& images) const;

		AbstractEngineRef engine;
		ThreadPoolRef thread_pool;
		VFSRef vfs;
	};
};