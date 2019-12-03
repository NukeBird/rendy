#include "texture_format.h"

std::string Rendy::to_string(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGB:
			return "RGB";
		case TextureFormat::RGBA:
			return "RGBA";
	}
	return "Invalid";
}