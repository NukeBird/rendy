#include "texture_format.h"
#include <optick.h>

std::string Rendy::to_string(TextureFormat format)
{
	OPTICK_EVENT();

	switch (format)
	{
		case TextureFormat::RGB:
			return "RGB";
		case TextureFormat::RGBA:
			return "RGBA";
	}

	return "Invalid";
}
