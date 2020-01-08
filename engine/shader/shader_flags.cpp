#include "shader_flags.h"
#include <limits>
#include <optick.h>

std::string Rendy::shader_flags_to_defines(uint32_t flags)
{
	OPTICK_EVENT();

	if (flags == NOTHING)
	{
		return "";
	}

	std::string result;

	if (flags & USE_VERTEX_POSITION)
	{
		result += "#define USE_VERTEX_POSITION\n";
	}

	if (flags & USE_VERTEX_COORD)
	{
		result += "#define USE_VERTEX_COORD\n";
	}
	
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

	/*if (flags & USE_VERTEX_TBN_MATRIX)
	{
		result += "#define USE_VERTEX_TBN_MATRIX\n";
	}*/

	if (flags & USE_VERTEX_BONES)
	{
		result += "#define USE_VERTEX_BONES\n";
	}

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

	return std::move(result);
}