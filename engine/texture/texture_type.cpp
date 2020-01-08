#include "texture_type.h"
#include <optick.h>

std::string Rendy::to_string(TextureType type)
{
	OPTICK_EVENT();

	if (type == TextureType::UnsignedByte)
	{
		return "UnsignedByte";
	}

	return "Invalid";
}
