#pragma once
#include "abstract_drawable.h"
#include "abstract_material.h"
#include "abstract_vertex_array.h"
#include "abstract_texture2d.h"
#include "abstract_shader.h"
#include "image2d.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <string>

namespace Rendy
{
	struct Mesh : public AbstractResource
	{
		AbstractVertexArrayRef vao;
		uint32_t material_id;
		std::string name;
		uint32_t flags;

		virtual void reload() override;
		virtual bool validate() const override;
	};

	struct Node
	{
		uint32_t root_id; //TODO: remove?
		std::string name;
		glm::mat4 transform;
		std::vector<uint32_t> mesh_ids;
		std::vector<uint32_t> child_ids;
	};

	class Model : public AbstractDrawable
	{
	public:
		uint32_t get_material_count() const;
		uint32_t get_mesh_count() const;
		uint32_t get_node_count() const;
		virtual void reload() override;
		virtual bool validate() const override;
		virtual BatchList generate_batch_list(const glm::mat4& model,
			const glm::mat4& view, const glm::mat4& proj) override;
	private:
		void generate_draw_calls(uint32_t node_id,
			const glm::mat4& base_transform,
			const glm::mat4& view,
			const glm::mat4& proj,
			BatchList& calls);

		std::vector<AbstractMaterialRef> materials;
		std::vector<Image2DRef> images;
		std::vector<Mesh> meshes;
		std::vector<Node> nodes;

		friend class ModelFactory;
	};

	using ModelRef = std::shared_ptr<Model>;
};