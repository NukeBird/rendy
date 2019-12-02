#pragma once
#include "../../ogl.hpp"
#include "abstract_texture2d.h"
#include <memory>
#include <string>

namespace Rendy
{
	class Texture2DFactory final
	{
	public:
		Texture2DFactory(OGL version);
		AbstractTexture2DRef make(Image2DRef image);
	private:
		OGL version;
	};
};