#include "batch.h"
#include "bind_shader.h"
#include "set_uniform.h"
#include "bind_vertex_array.h"
#include "draw.h"
#include <memory>

Rendy::CommandList Rendy::Batch::to_command_list() const
{
	auto list = material->to_command_list(extra_flags);

	auto shader_variant = material->get_shader_variant(extra_flags);
	list.emplace_back(std::make_shared<BindVertexArray>(vao, shader_variant));

	for (auto& u: uniforms)
	{
		list.emplace_back(u);
	}

	list.emplace_back(std::make_shared<Draw>(vao));

	return list;
}
