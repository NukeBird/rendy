#pragma once
#include <string>

enum class TextureFormat
{
	RGB,
	RGBA,
	Invalid
};

std::string to_string(TextureFormat format);