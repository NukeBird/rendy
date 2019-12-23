#pragma once
#include "../vertex_array/abstract_vertex_array.h"
#include "../command/abstract_command.h"
#include "../material/abstract_material.h"
#include "../shader/shader_source.h"
#include "../command/set_uniform.h"
#include <glm/glm.hpp>
#include <vector>

namespace Rendy
{
	struct Batch
	{
		AbstractVertexArrayRef vao;
		AbstractMaterialRef material;
		ShaderSettings shader_settings;
		std::vector<AbstractSetUniformRef> uniforms;

		CommandList to_command_list(ShaderSourceRef extra_source = nullptr) const;
	};

	using BatchList = std::vector<Batch>;
};