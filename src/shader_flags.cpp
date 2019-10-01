#include "shader_flags.h"
#include <limits>

uint32_t get_all_shader_flags()
{
	constexpr uint32_t flags = std::numeric_limits<uint32_t>::max();
	return flags;
}

std::string shader_flags_to_defines(uint32_t flags)
{
	if (flags == NOTHING)
	{
		return "";
	}

	std::string result;
	
	if (flags & USE_VERTEX_COLOR)
	{
		result += "#define USE_VERTEX_COLOR\n";
	}

	if (flags & USE_VERTEX_NORMAL)
	{
		result += "#define USE_VERTEX_NORMAL\n";
	}

	if (flags & USE_VERTEX_TANGENT)
	{
		result += "#define USE_VERTEX_TANGENT\n";
	}

	if (flags & USE_VERTEX_BITANGENT)
	{
		result += "#define USE_VERTEX_BITANGENT\n";
	}

	/*if (flags & USE_VERTEX_BONES)
	{
		result += "#define USE_VERTEX_BONES\n";
	}*/

	if (flags & USE_COLOR_TEXTURE)
	{
		result += "#define USE_COLOR_TEXTURE\n";
	}

	if (flags & USE_NORMAL_TEXTURE)
	{
		result += "#define USE_NORMAL_TEXTURE\n";
	}

	if (flags & USE_AMBIENT_TEXTURE)
	{
		result += "#define USE_AMBIENT_TEXTURE\n";
	}

	if (flags & USE_SPECULAR_TEXTURE)
	{
		result += "#define USE_SPECULAR_TEXTURE\n";
	}

	if (flags & USE_METALLIC_ROUGHNESS_TEXTURE)
	{
		result += "#define USE_METALLIC_ROUGHNESS_TEXTURE\n";
	}

	return result;
}
