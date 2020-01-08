#include "batch.h"
#include "../command/bind_shader.h"
#include "../command/set_uniform.h"
#include "../command/bind_vertex_array.h"
#include "../command/draw.h"
#include <memory>
#include <optick.h>

Rendy::CommandList Rendy::Batch::to_command_list(ShaderSourceRef extra_source) const
{
	OPTICK_EVENT();

	auto list = material->to_command_list(shader_settings, extra_source);
	auto shader_variant = material->get_shader_variant(shader_settings, extra_source);
	list.emplace_back(std::make_shared<BindVertexArray>(vao, shader_variant));

	for (auto& v3: vec3_uniforms)
	{
		list.emplace_back(std::make_shared<SetUniformVec3>(shader_variant, v3.first, v3.second));
	}

	for (auto& m4: mat4_uniforms)
	{
		list.emplace_back(std::make_shared<SetUniformMat4>(shader_variant, m4.first, m4.second));
	}

	for (auto& m3: mat3_uniforms)
	{
		list.emplace_back(std::make_shared<SetUniformMat3>(shader_variant, m3.first, m3.second));
	}

	for (auto& f: float_uniforms)
	{
		list.emplace_back(std::make_shared<SetUniformFloat>(shader_variant, f.first, f.second));
	}

	for (auto& i: int_uniforms)
	{
		list.emplace_back(std::make_shared<SetUniformInt>(shader_variant, i.first, i.second));
	}

	for (auto& m4a: mat4_uniform_arrays)
	{
		list.emplace_back(std::make_shared<SetUniformMat4Array>(shader_variant, m4a.first, 
			m4a.second));
	}

	list.emplace_back(std::make_shared<Draw>(vao));

	return list;
}
