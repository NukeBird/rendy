#include "texture_type.h"

std::string Rendy::to_string(TextureType type)
{
	if (type == TextureType::UnsignedByte)
	{
		return "UnsignedByte";
	}

	return "Invalid";
}
