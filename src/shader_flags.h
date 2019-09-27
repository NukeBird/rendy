#pragma once
#include <string>
#include "smart_enum.hpp"

smart_enum(ShaderFlag, //TODO!!! operator| and operator& works wrong (probably stfu smart_enum)
	NOTHING, //1 << 0
	USE_VERTEX_COLOR, //1 << 1
	USE_VERTEX_NORMAL, //...
	USE_VERTEX_TANGENT,
	USE_VERTEX_BITANGENT,
	USE_VERTEX_BONES,
	USE_COLOR_TEXTURE,
	USE_NORMAL_TEXTURE,
	USE_AMBIENT_TEXTURE,
	USE_SPECULAR_TEXTURE,
	USE_METALLIC_ROUGHNESS_TEXTURE);

inline ShaderFlag operator|(ShaderFlag a, ShaderFlag b);
inline bool operator&(ShaderFlag a, ShaderFlag b);

ShaderFlag get_all_shader_flags();

std::string shader_flags_to_defines(ShaderFlag flags);