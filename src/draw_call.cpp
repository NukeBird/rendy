#include "draw_call.h"
#include "bind_shader.h"
#include "set_uniform.h"
#include "bind_vertex_array.h"
#include "draw.h"
#include <memory>

std::vector<CommandRef> DrawCall::to_command_list()
{
	auto list = material->to_command_list(extra_flags);

	auto shader_variant = material->get_shader_variant(extra_flags);
	list.emplace_back(std::make_shared<BindVertexArray>(vao, shader_variant));
	list.emplace_back(std::make_shared<SetUniform<glm::mat4>>(
		shader_variant, "u_view", view));
	list.emplace_back(std::make_shared<SetUniform<glm::mat4>>(
		shader_variant, "u_projection", proj));
	list.emplace_back(std::make_shared<SetUniform<glm::mat4>>(
		shader_variant, "u_view_projection", proj * view));
	list.emplace_back(std::make_shared<SetUniform<glm::mat4>>(
		shader_variant, "u_transform", model));

	glm::mat3 view_rotation(view);
	glm::vec3 camera_position = -view[3] * view_rotation;

	list.emplace_back(std::make_shared<SetUniform<glm::vec3>>(
		shader_variant, "u_camera_position", camera_position));

	list.emplace_back(std::make_shared<Draw>(vao));

	return list;
}
