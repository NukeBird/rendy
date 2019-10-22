#pragma once
#include "abstract_vertex_array.h"
#include "abstract_texture2d.h"
#include "abstract_material.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>

struct MeshS//TODO: rename
{
	AbstractVertexArrayRef vao;
	uint32_t material_id;
	std::string name;
	uint32_t flags;
};

enum AnimationBehaviour
{
	DEFAULT,	//The value from the default node transformation is taken.

	CONSTANT,	//The nearest key value is used without interpolation.

	LINEAR,		//The value of the nearest two keys is linearly extrapolated 
				//for the current time value.

	REPEAT,		//The animation is repeated. f the animation key go from n to m 
				//and the current time is t, use the value at (t-n) % (|m-n|).

};

struct NodeAnimationS //TODO: rename
{
	std::string name; //The name of the node affected by this animation.
	AnimationBehaviour post_state; //how the animation behaves after the last key was processed.
	AnimationBehaviour pre_state; //how the animation behaves before the first key is encountered.
	std::vector<glm::vec3> position_keys;
	std::vector<glm::vec3> scaling_keys;
	std::vector<glm::quat> rotation_keys;
};

struct AnimationS //TODO: rename
{
	std::string name;

	float duration;
	float ticks_per_sec;
};

struct CameraS //TODO: rename
{
	std::string name;
	float near;
	float far;
	float fov;
	glm::vec3 look_at;	//relative to parent node
	glm::vec3 position;	//relative to parent node
	glm::vec3 up;		//relative to parent node
};

struct NodeS //TODO: rename
{
	using NodeRef = std::shared_ptr<NodeS>;

	std::string name;
	glm::mat4 transform;
	std::vector<uint32_t> mesh_ids;
	std::vector<NodeRef> child_list;
};

struct DirectLightS
{
	DirectLightS(const glm::vec3& color, const glm::vec3& direction)
	{
		this->color = color;
		this->direction = direction;
	}

	glm::vec3 color;
	glm::vec3 direction;
};

struct Scene //TODO: struct?
{
	std::vector<MeshS> mesh_list;
	std::vector<AbstractTextureRef> texture_list;
	std::vector<AbstractMaterialRef> material_list;
	std::vector<CameraS> camera_list;
	std::vector<AnimationS> animation_list;
	std::vector<DirectLightS> direct_light_list;
	std::unordered_map<std::string, NodeS::NodeRef> node_map;
	NodeS::NodeRef root_node;
};

using SceneRef = std::shared_ptr<Scene>;