#pragma once
#include <string>

namespace Rendy
{
	enum ShaderFlag
	{
		NOTHING = 0,
		USE_VERTEX_POSITION = 1 << 1,
		USE_VERTEX_COORD = 1 << 2,
		USE_VERTEX_COLOR = 1 << 3,
		USE_VERTEX_NORMAL = 1 << 4,
		USE_VERTEX_TANGENT = 1 << 5,
		USE_VERTEX_BITANGENT = 1 << 6,
		//USE_VERTEX_TBN_MATRIX = 1 << 7,
		USE_VERTEX_BONES = 1 << 8,
		USE_COLOR_TEXTURE = 1 << 9,
		USE_NORMAL_TEXTURE = 1 << 10,
		USE_AMBIENT_TEXTURE = 1 << 11,
		USE_SPECULAR_TEXTURE = 1 << 12,
		USE_METALLIC_ROUGHNESS_TEXTURE = 1 << 13,
	};

	uint32_t get_all_shader_flags();

	std::string shader_flags_to_defines(uint32_t flags);
};