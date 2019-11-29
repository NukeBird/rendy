#pragma once
#include <string>

namespace Rendy
{
	enum class TextureFormat
	{
		RGB,
		RGBA,
		Invalid
	};

	std::string to_string(TextureFormat format);
};