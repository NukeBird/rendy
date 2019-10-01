#pragma once
#include <string>

enum ShaderFlag
{
	NOTHING = 0, 
	USE_VERTEX_COLOR = 1 << 1,
	USE_VERTEX_NORMAL = 1 << 2,
	USE_VERTEX_TANGENT = 1 << 3,
	USE_VERTEX_BITANGENT = 1 << 4,
	USE_VERTEX_BONES = 1 << 5,
	USE_COLOR_TEXTURE = 1 << 6,
	USE_NORMAL_TEXTURE = 1 << 7,
	USE_AMBIENT_TEXTURE = 1 << 8,
	USE_SPECULAR_TEXTURE = 1 << 9,
	USE_METALLIC_ROUGHNESS_TEXTURE = 1 << 10,
};

uint32_t get_all_shader_flags();

std::string shader_flags_to_defines(uint32_t flags);