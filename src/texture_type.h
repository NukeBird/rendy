#pragma once
#include <string>

enum class TextureType
{
	UnsignedByte,
	Invalid
};

std::string to_string(TextureType type);