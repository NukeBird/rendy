#pragma once
#include "abstract_drawable.h"
#include "abstract_material.h"
#include "abstract_vertex_array.h"
#include "abstract_texture.h"
#include "abstract_shader.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct Mesh: public AbstractResource
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

class Model: public AbstractDrawable
{
public:
	uint32_t get_material_count() const;
	uint32_t get_mesh_count() const;
	uint32_t get_node_count() const;
	virtual void reload() override;
	virtual bool validate() const override;	
	
	virtual DrawCallList generate_draw_calls(const glm::mat4& model,
		const glm::mat4& view, const glm::mat4& proj) override;

	virtual void draw(const glm::mat4& transform = glm::mat4(1.0),
		const glm::mat4& view = glm::mat4(1.0),
		const glm::mat4& proj = glm::mat4(1.0)) override;
private:
	void draw_node(uint32_t node_id, 
		const glm::mat4& base_transform = glm::mat4(1.0),
		const glm::mat4& view = glm::mat4(1.0),
		const glm::mat4& proj = glm::mat4(1.0));

	void generate_draw_calls(uint32_t node_id,
		const glm::mat4& base_transform,
		const glm::mat4& view,
		const glm::mat4& proj,
		DrawCallList& calls);

	std::vector<AbstractMaterialRef> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;

	friend class ModelBuilder;
};

using ModelRef = std::shared_ptr<Model>;