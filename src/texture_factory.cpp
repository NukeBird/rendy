#include "texture_factory.h"
#include "es2/texture.h"
#include <optick.h>

AbstractTexture2DRef TextureFactory::make(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", length);
	auto texture = std::make_shared<ES2::Texture>(memory, length);
	return texture;
}