#pragma once
#include "abstract_texture2d.h"
#include "singleton.h"
#include <vector>
#include <memory>

class TextureFactory final: public Singleton<TextureFactory>
{
public:
	//TODO: make from memory?
	AbstractTexture2DRef make(const char* memory, uint32_t length);
};