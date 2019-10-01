#pragma once
#include "abstract_drawable.h"
#include "abstract_material.h"
#include "abstract_vertex_array.h"
#include "abstract_texture.h"
#include "abstract_shader.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/config.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
	virtual void draw() override; //TODO: pass transform
private:
	void draw_node(uint32_t node_id, 
		const glm::mat4& base_transform = glm::mat4(1.0));

	std::vector<AbstractMaterialRef> materials;
	std::vector<Mesh> meshes;
	std::vector<Node> nodes;

	friend class ModelBuilder;
};