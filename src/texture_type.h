#pragma once
#include <string>

namespace Rendy
{
	enum class TextureType
	{
		UnsignedByte,
		HalfFloat,
		Invalid
	};

	std::string to_string(TextureType type);
};