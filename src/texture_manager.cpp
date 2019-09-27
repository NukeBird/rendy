#include "texture_manager.h"
#include "es2/texture.h"
#include <algorithm>

AbstractTextureRef TextureManager::make(const char* memory, uint32_t length)
{
	auto texture = std::make_shared<ES2::Texture>(memory, length);
	texture_list.emplace_back(texture);
	return texture;
}

void TextureManager::reload()
{
	for (auto& texture : texture_list)
	{
		texture->reload();
	}
}

void TextureManager::flush()
{
	const auto rule = [](const AbstractTextureRef& texture)
	{
		return texture.use_count() == 1 || !texture->validate();
	};

	auto remove_it = std::remove_if(texture_list.begin(),
		texture_list.end(), rule);

	texture_list.erase(remove_it);
}
