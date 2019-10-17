#include "es2_engine.h"
#include <memory>

void ES2Engine::render(const SceneRef& scene, const glm::mat4& model, const glm::mat4& view,
	const glm::mat4& proj)
{
	auto draw_calls = form_draw_calls(scene, model, view, proj);

	glm::mat3 view_rotation(view);
	glm::vec3 camera_position = -view[3] * view_rotation;

	for (auto& dc: draw_calls)
	{
		render(dc, scene, camera_position);
	}
}

std::vector<DrawCall> ES2Engine::form_draw_calls(const SceneRef& scene, const glm::mat4& model,
	const glm::mat4& view, const glm::mat4& proj)
{
	std::vector<DrawCall> draw_calls;

	form_draw_calls(scene, scene->root_node, model, 
		view, proj, draw_calls);

	return std::move(draw_calls);
}

void ES2Engine::form_draw_calls(const SceneRef& scene, const NodeS::NodeRef& node,
	const glm::mat4 & model, const glm::mat4& view, const glm::mat4& proj, 
	std::vector<DrawCall>& draw_calls)
{
	if (!node)
	{
		return;
	}

	auto transform = model * node->transform;

	for (const auto& mesh_id : node->mesh_ids)
	{
		assert(mesh_id < get_mesh_count());
		auto& mesh = scene->mesh_list[mesh_id];

		assert(mesh.material_id < get_material_count());
		auto& material = scene->material_list[mesh.material_id];

		draw_calls.emplace_back();
		auto& call = draw_calls[draw_calls.size() - 1];
		call.vao = mesh.vao;
		call.extra_flags = mesh.flags;
		call.material = material;
		call.model = transform;
		call.view = view;
		call.proj = proj;
	}

	for (const auto& child_id : node->child_list)
	{
		form_draw_calls(scene, child_id, transform, view, proj, draw_calls);
	}
}

void ES2Engine::render(const DrawCall& draw_call,
	const SceneRef& scene, const glm::vec3& camera_position)
{
	auto material = draw_call.material;

	auto shader = material->get_shader();

	ShaderSettings settings; //TODO: lights
	settings.flags = draw_call.extra_flags | material->get_flags();

	/*if (!scene->direct_light_list.empty())
	{
		settings.flags |= ShaderFlag::USE_DIRECT_LIGHTS;
		settings.properties[ShaderProperty::DIRECT_LIGHT_COUNT] = 
			static_cast<uint32_t>(scene->direct_light_list.size());
	}*/

	auto shader_variant = shader->compile(settings); //TODO

	//if (!shader_variant->validate())
	{
		//printf("NOT VALID\n");
	}

	draw_call.vao->bind(shader_variant);
	//printf("%d\n", material->validate());
	material->bind(settings);
	shader_variant->set_uniform("u_view", draw_call.view);
	shader_variant->set_uniform("u_projection", draw_call.proj);
	shader_variant->set_uniform("u_view_projection", draw_call.proj *
		draw_call.view);
	shader_variant->set_uniform("u_transform", draw_call.model);
	shader_variant->set_uniform("u_camera_position", camera_position);

	/*for (uint32_t i = 0; i < scene->direct_light_list.size(); ++i)
	{
		auto& light = scene->direct_light_list[i];

		auto meta = "direct_lights[" + std::to_string(i) + "]";
		shader_variant->set_uniform(meta  + ".color", light.color);
		shader_variant->set_uniform(meta + ".direction", light.direction);
	}*/

	draw_call.vao->draw();
	draw_call.vao->unbind();
	material->unbind(draw_call.extra_flags);
}
