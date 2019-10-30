#pragma once
#include <string>

namespace Rendy
{
	enum class TextureType
	{
		UnsignedByte,
		Invalid
	};

	std::string to_string(TextureType type);
};