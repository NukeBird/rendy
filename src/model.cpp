#include "model.h"
#include <stack>
#include <optick.h>
#include <glm/gtx/quaternion.hpp>
#include "es2/texture_cube.h"
#include "common.h"

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
		generate_draw_calls(0, glm::mat4(1.0), model, view, proj, calls);
	}
	return std::move(calls);
}

glm::mat4 Rendy::Model::get_world_transform(const Node& node) const
{
	if (node.root_id == -1)
	{
		return node.transform;
	}

	return get_world_transform(nodes[node.root_id]) * node.transform;
}

std::vector<glm::mat4> Rendy::Model::calculate_bone_transforms(const Mesh& mesh,
	const glm::mat4& inverse_transform)
{
	std::vector<glm::mat4> transforms;

	auto& mesh_bones = name_to_bones[mesh.name];
	//transforms.reserve(mesh_bones.size());
	transforms.resize(mesh_bones.size(), glm::mat4(1));

	uint32_t i = 0;

	for (const auto& bone: mesh_bones)
	{
		auto bone_node_id = name_to_node[bone.name];
		auto& bone_node = nodes[bone_node_id];
		transforms[i] = (inverse_transform * get_world_transform(bone_node) *
			bone.offset_matrix);

		/*if (bone_node_id >= 120)
		{
			printf("REE\n");
		}*/

		++i;
	}

	return std::move(transforms); //TODO?
}

void Rendy::Model::generate_draw_calls(uint32_t node_id, const glm::mat4& base_transform,
	const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj, BatchList& calls)
{
	OPTICK_EVENT();
	auto& node = nodes[node_id];

	glm::mat4 transform = base_transform * node.transform;

	if (!node.mesh_ids.empty())
	{
		glm::mat4 inverse_transform = glm::inverse(transform);

		for (const auto& mesh_id : node.mesh_ids)
		{
			assert(mesh_id < get_mesh_count());
			auto& mesh = meshes[mesh_id];

			assert(mesh.material_id < get_material_count());
			auto& material = materials[mesh.material_id];

			auto bones = calculate_bone_transforms(mesh, inverse_transform);

			calls.emplace_back();
			auto& call = calls[calls.size() - 1];
			call.vao = mesh.vao;
			call.extra_flags = mesh.flags;
			call.material = material;
			auto shader_variant = material->get_shader_variant(call.extra_flags);

			call.uniforms.emplace_back(std::make_shared<SetUniformMat4Array>
				(shader_variant, "u_bones[0]", bones));

			call.uniforms.emplace_back(std::make_shared<SetUniformMat4>
				(shader_variant, "u_transform", model * transform));
			call.uniforms.emplace_back(std::make_shared<SetUniformMat4>
				(shader_variant, "u_view", view));
			call.uniforms.emplace_back(std::make_shared<SetUniformMat4>
				(shader_variant, "u_projection", proj));
			call.uniforms.emplace_back(std::make_shared<SetUniformMat4>
				(shader_variant, "u_view_projection", proj * view));	
			
			glm::mat3 view_rotation(view);
			glm::vec3 camera_position = -view[3] * view_rotation;

			call.uniforms.emplace_back(std::make_shared<SetUniform<glm::vec3>>(
				shader_variant, "u_camera_position", camera_position));
		}
	}

	for (const auto& child_id : node.child_ids)
	{
		generate_draw_calls(child_id, transform, model, view, proj, calls);
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
	if (animation->position_keys.size() == 1)
	{
		return animation->position_keys[0].value;
	}

	uint32_t position_index = find_key_index(animation->position_keys, time);
	uint32_t next_position_index = position_index + 1;
	assert(next_position_index < static_cast<uint32_t>(animation->position_keys.size()));
	float dt = animation->position_keys[next_position_index].time -
		animation->position_keys[position_index].time;
	assert(dt >= 0.0f);
	float factor = (time - animation->position_keys[position_index].time) / dt;
	assert(factor >= 0.0f && factor <= 1.0f);
	const auto& start = animation->position_keys[position_index].value;
	const auto& end = animation->position_keys[next_position_index].value;
	const auto delta_position = end - start;

	return start + factor * delta_position;
}

glm::quat Rendy::Model::calculate_rotation(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	if (animation->rotation_keys.size() == 1)
	{
		return animation->rotation_keys[0].value;
	}

	uint32_t rotation_index = find_key_index(animation->rotation_keys, time);
	uint32_t next_rotation_index = (rotation_index + 1);
	assert(next_rotation_index < static_cast<uint32_t>(animation->rotation_keys.size()));
	float dt = animation->rotation_keys[next_rotation_index].time - 
		animation->rotation_keys[rotation_index].time;
	float factor = (time - animation->rotation_keys[rotation_index].time) / dt;
	assert(factor >= 0.0f && factor <= 1.0f);
	const auto& start = animation->rotation_keys[rotation_index].value;
	const auto& end = animation->rotation_keys[next_rotation_index].value;

	return glm::normalize(glm::slerp(start, end, factor));
}

glm::vec3 Rendy::Model::calculate_scale(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();
	if (animation->scaling_keys.size() == 1)
	{
		return animation->scaling_keys[0].value;
	}

	uint32_t scaling_index = find_key_index(animation->scaling_keys, time);
	uint32_t next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < static_cast<uint32_t>(animation->scaling_keys.size()));
	float dt = animation->scaling_keys[next_scaling_index].time -
		animation->scaling_keys[scaling_index].time;
	assert(dt >= 0.0f);
	float factor = (time - animation->scaling_keys[scaling_index].time) / dt;
	assert(factor >= 0.0f && factor <= 1.0f);
	const auto& start = animation->scaling_keys[scaling_index].value;
	const auto& end = animation->scaling_keys[next_scaling_index].value;
	const auto delta_scaling = end - start;

	return start + factor * delta_scaling;
}

glm::mat4 Rendy::Model::calculate_transform(AnimationNodeRef animation, float time)
{
	OPTICK_EVENT();

	auto position = calculate_position(animation, time);
	auto rotation = calculate_rotation(animation, time);
	auto scale = calculate_scale(animation, time);

	glm::mat4 transform = 
		glm::translate(position) *
		glm::toMat4(rotation) *
		glm::scale(scale);

	return std::move(transform); //TODO?
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
