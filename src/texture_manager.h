#pragma once
#include "abstract_texture.h"
#include "singleton.h"
#include <vector>
#include <memory>

class TextureManager final: public Singleton<TextureManager>
{
public:
	//TODO: make from memory?
	AbstractTextureRef make(const char* memory, uint32_t length);
	void reload();
	void flush();
private:
	std::vector<AbstractTextureRef> texture_list;
};