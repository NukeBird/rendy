#include "model.h"
#include <stack>

void Mesh::reload()
{
	if (vao)
	{
		vao->reload();
	}
}

bool Mesh::validate() const
{
	if (vao)
	{
		return vao->validate();
	}

	return false;
}

uint32_t Model::get_material_count() const
{
	return static_cast<uint32_t>(materials.size());
}

uint32_t Model::get_mesh_count() const
{
	return static_cast<uint32_t>(meshes.size());
}

uint32_t Model::get_node_count() const
{
	return static_cast<uint32_t>(nodes.size());
}

void Model::reload()
{
	for (auto& material: materials)
	{
		material->reload();
	}

	for (auto& mesh: meshes)
	{
		mesh.reload();
	}
}

bool Model::validate() const
{
	for (auto& material: materials)
	{
		if (!material->validate())
		{
			return false;
		}
	}

	for (auto& mesh: meshes)
	{
		if (!mesh.validate())
		{
			return false;
		}
	}

	//TODO: something missed?

	return true;
}

DrawCallList Model::generate_draw_calls(const glm::mat4& model, const glm::mat4& view, 
	const glm::mat4& proj)
{
	DrawCallList calls;
	if (get_node_count() > 0)
	{
		generate_draw_calls(0, model, view, proj, calls);
	}
	return std::move(calls);
}

void Model::draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj)
{
	auto calls = generate_draw_calls(transform, view, proj);

	glm::mat3 view_rotation(view);
	glm::vec3 camera_position = -view[3] * view_rotation;

	for (auto& draw_call : calls)
	{
		auto material = draw_call.material;

		auto shader = material->get_shader();

		ShaderSettings settings; //TODO: lights
		settings.flags = draw_call.extra_flags | material->get_flags();
		auto shader_variant = shader->compile(settings); //TODO

		draw_call.vao->bind(shader_variant);

		material->bind(draw_call.extra_flags);
		shader_variant->set_uniform("u_view", draw_call.view);
		shader_variant->set_uniform("u_projection", draw_call.proj);
		shader_variant->set_uniform("u_view_projection", draw_call.proj * 
			draw_call.view);
		shader_variant->set_uniform("u_transform", draw_call.model);
		shader_variant->set_uniform("u_camera_position", camera_position);

		draw_call.vao->draw();
		draw_call.vao->unbind();
		material->unbind(draw_call.extra_flags);
	}
}

void Model::draw_node(uint32_t node_id,
	const glm::mat4& base_transform, const glm::mat4& view, const glm::mat4& proj)
{
	assert(node_id < get_node_count());
	auto& node = nodes[node_id];

	auto transform = base_transform * node.transform;

	glm::mat3 view_rotation(view);
	glm::vec3 camera_position = -view[3] * view_rotation;

	for (const auto& mesh_id: node.mesh_ids)
	{
		assert(mesh_id < get_mesh_count());
		auto& mesh = meshes[mesh_id];

		assert(mesh.material_id < get_material_count());
		auto& material = materials[mesh.material_id];

		auto shader = material->get_shader();

		ShaderSettings settings; //TODO: lights
		settings.flags = mesh.flags | material->get_flags();
		auto shader_variant = shader->compile(settings); //TODO

		mesh.vao->bind(shader_variant);

		material->bind(mesh.flags);
		shader_variant->set_uniform("u_view", view);
		shader_variant->set_uniform("u_projection", proj);
		shader_variant->set_uniform("u_view_projection", proj * view);
		shader_variant->set_uniform("u_transform", transform);
		shader_variant->set_uniform("u_camera_position", camera_position);

		mesh.vao->draw();
		mesh.vao->unbind();
		material->unbind(mesh.flags);
	}

	for (const auto& child_id: node.child_ids)
	{
		draw_node(child_id, transform, view, proj);
	}
}

void Model::generate_draw_calls(uint32_t node_id, const glm::mat4& base_transform, 
	const glm::mat4& view, const glm::mat4& proj, DrawCallList& calls)
{
	auto& node = nodes[node_id];

	auto transform = base_transform * node.transform;

	for (const auto& mesh_id: node.mesh_ids)
	{
		assert(mesh_id < get_mesh_count());
		auto& mesh = meshes[mesh_id];

		assert(mesh.material_id < get_material_count());
		auto& material = materials[mesh.material_id];

		calls.emplace_back();
		auto& call = calls[calls.size() - 1];
		call.vao = mesh.vao;
		call.extra_flags = mesh.flags;
		call.material = material;
		call.model = transform;
		call.view = view;
		call.proj = proj;
	}

	for (const auto& child_id : node.child_ids)
	{
		generate_draw_calls(child_id, transform, view, proj, calls);
	}
}
