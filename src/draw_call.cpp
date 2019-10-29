#include "draw_call.h"
#include "bind_shader.h"
#include <memory>

std::vector<CommandRef> DrawCall::to_command_list()
{
	auto shader = material->get_shader();
	ShaderSettings settings;
	settings.flags = extra_flags | material->get_flags();

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(shader, settings));

	return std::vector<CommandRef>();
}
