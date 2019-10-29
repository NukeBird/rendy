#pragma once
#include "abstract_vertex_array.h"
#include "abstract_command.h"
#include "abstract_material.h"
#include <glm/glm.hpp>

struct DrawCall
{
	AbstractVertexArrayRef vao;
	AbstractMaterialRef material;

	uint32_t extra_flags = 0;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};