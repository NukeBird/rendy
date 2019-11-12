#include "model.h"
#include <stack>
#include <optick.h>

#include "es2/texture_cube.h"

void Rendy::Mesh::reload()
{
	OPTICK_EVENT();

	if (vao)
	{
		vao->reload();
	}
}

bool Rendy::Mesh::validate() const
{
	OPTICK_EVENT();

	if (vao)
	{
		return vao->validate();
	}

	return false;
}

bool Rendy::Model::has_animations() const
{
	OPTICK_EVENT();
	return !animations.empty();
}

uint32_t Rendy::Model::get_material_count() const
{
	OPTICK_EVENT();
	return static_cast<uint32_t>(materials.size());
}

uint32_t Rendy::Model::get_mesh_count() const
{
	OPTICK_EVENT();
	return static_cast<uint32_t>(meshes.size());
}

uint32_t Rendy::Model::get_node_count() const
{
	OPTICK_EVENT();
	return static_cast<uint32_t>(nodes.size());
}

void Rendy::Model::update(float dt)
{
	OPTICK_EVENT();

	elapsed_time += dt;

	if (has_animations())
	{
		auto& current_animation = animations[animation_index];
		
		float time_in_ticks = elapsed_time * 
			current_animation.ticks_per_second;

		float animation_time = glm::mod(time_in_ticks, current_animation.duration);

		for(auto& channel: current_animation.channels)
		{
			auto it = name_to_node.find(channel->name);

			if (it == name_to_node.end())
			{
				printf("Can't find %s node\n", channel->name.c_str());
				continue;
			}

			auto& node = nodes[it->second];

			node.transform = std::move(calculate_transform(channel, animation_time));
		}
	}
}

void Rendy::Model::reload()
{
	OPTICK_EVENT();
	for (auto& material: materials)
	{
		material->reload();
	}

	for (auto& mesh: meshes)
	{
		mesh.reload();
	}
}

bool Rendy::Model::validate() const
{
	OPTICK_EVENT();
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

Rendy::BatchList Rendy::Model::generate_batch_list(const glm::mat4& model, const glm::mat4& view,
	const glm::mat4& proj)
{
	OPTICK_EVENT();
	BatchList calls;
	if (get_node_count() > 0)
	{
		generate_draw_calls(0, model, view, proj, calls);
	}
	return std::move(calls);
}

void Rendy::Model::generate_draw_calls(uint32_t node_id, const glm::mat4& base_transform,
	const glm::mat4& view, const glm::mat4& proj, BatchList& calls)
{
	OPTICK_EVENT();
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

template<class T>
static uint32_t find_key_index(const std::vector<T>& keys, float time)
{
	OPTICK_EVENT();

	for (uint32_t i = 0; i < static_cast<uint32_t>(keys.size()) - 1; ++i)
	{
		const auto& key = keys[i + 1];

		if (time < key.time)
		{
			return i;
		}
	}

	return 0;
}

uint32_t Rendy::Model::find_key_index(const std::vector<VectorKey>& keys, float time) const
{
	OPTICK_EVENT();
	return ::find_key_index(keys, time);
}

uint32_t Rendy::Model::find_key_index(const std::vector<QuatKey>& keys, float time) const
{
	OPTICK_EVENT();
	return ::find_key_index(keys, time);
}

glm::vec3 Rendy::Model::calculate_position(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	return glm::vec3();
}

glm::quat Rendy::Model::calculate_rotation(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	return glm::quat();
}

glm::vec3 Rendy::Model::calculate_scale(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	return glm::vec3();
}

glm::mat4 Rendy::Model::calculate_transform(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	return glm::mat4();
}

void Rendy::Model::calculate_cache()
{
	OPTICK_EVENT();

	for (uint32_t i = 0; i < static_cast<uint32_t>(nodes.size()); ++i)
	{
		auto& node = nodes[i];
		auto it = name_to_node.find(node.name);

		if (it != name_to_node.end())
		{
			printf("NODE NAME DUPLICATION: %s\n", node.name.c_str());
		}
		else
		{
			name_to_node[node.name] = i;
			printf("Node %s cached\n", node.name.c_str());
		}
	}
}
