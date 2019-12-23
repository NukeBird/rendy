#pragma once
#include "../vertex_array/abstract_vertex_array.h"
#include "../command/abstract_command.h"
#include "../material/abstract_material.h"
#include "../shader/shader_source.h"
#include "../command/set_uniform.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

namespace Rendy
{
	template<class T>
	using UniformList = std::unordered_map<std::string, T>;

	struct Batch
	{
		AbstractVertexArrayRef vao;
		AbstractMaterialRef material;
		ShaderSettings shader_settings;
		//std::vector<AbstractSetUniformRef> uniforms;
		UniformList<glm::vec3> vec3_uniforms;
		UniformList<glm::mat4> mat4_uniforms;
		UniformList<glm::mat3> mat3_uniforms;
		UniformList<float> float_uniforms;
		UniformList<int> int_uniforms;
		UniformList<Mat4Array> mat4_uniform_arrays;

		CommandList to_command_list(ShaderSourceRef extra_source = nullptr) const;
	};

	using BatchList = std::vector<Batch>;
};