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

void Model::draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj)
{
	if (get_node_count() > 0)
	{
		draw_node(0, transform, view, proj);
	}
}

void Model::draw_node(uint32_t node_id,
	const glm::mat4& base_transform, const glm::mat4& view, const glm::mat4& proj)
{
	assert(node_id < get_node_count());
	auto& node = nodes[node_id];

	auto transform = node.transform * base_transform;
	
	for (const auto& mesh_id: node.mesh_ids)
	{
		assert(mesh_id < get_mesh_count());
		auto& mesh = meshes[mesh_id];

		assert(mesh.material_id < get_material_count());
		auto& material = materials[mesh.material_id];

		auto shader = material->get_shader();
		auto shader_variant = shader->compile(mesh.flags | material->get_flags()); //TODO

		mesh.vao->bind(shader_variant);

		material->bind(mesh.flags);
		shader_variant->set_uniform("u_view", view);
		shader_variant->set_uniform("u_projection", proj);
		shader_variant->set_uniform("u_view_projection", proj * view);
		shader_variant->set_uniform("u_transform", transform);

		mesh.vao->draw();
		mesh.vao->unbind();
		material->unbind(mesh.flags);
	}

	for (const auto& child_id: node.child_ids)
	{
		draw_node(child_id, transform);
	}
}
