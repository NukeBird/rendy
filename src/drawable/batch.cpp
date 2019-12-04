#include "batch.h"
#include "../command/bind_shader.h"
#include "../command/set_uniform.h"
#include "../command/bind_vertex_array.h"
#include "../command/draw.h"
#include <memory>
#include <optick.h>

Rendy::CommandList Rendy::Batch::to_command_list() const
{
	OPTICK_EVENT();

	auto list = material->to_command_list(shader_settings);
	auto shader_variant = material->get_shader_variant(shader_settings);
	list.emplace_back(std::make_shared<BindVertexArray>(vao, shader_variant));

	for (auto& u: uniforms)
	{
		list.emplace_back(u);
	}

	list.emplace_back(std::make_shared<Draw>(vao));

	return list;
}
