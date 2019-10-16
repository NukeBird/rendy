#include "texture_factory.h"
#include "es2/texture.h"

AbstractTextureRef TextureFactory::make(const char* memory, uint32_t length)
{
	auto texture = std::make_shared<ES2::Texture>(memory, length);
	return texture;
}