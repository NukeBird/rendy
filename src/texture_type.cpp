#include "texture_type.h"

std::string to_string(TextureType type)
{
	if (type == TextureType::UnsignedByte)
	{
		return "UnsignedByte";
	}

	return "Invalid";
}
