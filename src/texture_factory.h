#pragma once
#include "abstract_texture.h"
#include "singleton.h"
#include <vector>
#include <memory>

class TextureFactory final: public Singleton<TextureFactory>
{
public:
	//TODO: make from memory?
	AbstractTextureRef make(const char* memory, uint32_t length);
};