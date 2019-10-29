#pragma once
#include "abstract_vertex_array.h"
#include "abstract_command.h"
#include "abstract_material.h"
#include <glm/glm.hpp>
#include <vector>

struct Batch
{
	AbstractVertexArrayRef vao;
	AbstractMaterialRef material;

	uint32_t extra_flags = 0;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

	std::vector<CommandRef> to_command_list();
};