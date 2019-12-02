#pragma once
#include "vertex_array/abstract_vertex_array.h"
#include "command/abstract_command.h"
#include "material/abstract_material.h"
#include "command/set_uniform.h"
#include <glm/glm.hpp>
#include <vector>

namespace Rendy
{
	struct Batch
	{
		AbstractVertexArrayRef vao;
		AbstractMaterialRef material;

		std::vector<AbstractSetUniformRef> uniforms;

		uint32_t extra_flags = 0;

		CommandList to_command_list() const;
	};

	using BatchList = std::vector<Batch>;
};