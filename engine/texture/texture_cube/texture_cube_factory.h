#pragma once
#include "../../ogl.hpp"
#include "abstract_texture_cube.h"
#include <memory>
#include <string>

namespace Rendy
{
	class TextureCubeFactory final
	{
	public:
		TextureCubeFactory(OGL version);
		AbstractTextureCubeRef make(const std::string& filename);
		AbstractTextureCubeRef make(const void* memory, uint32_t size);
	private:
		OGL version;
	};

	using TextureCubeFactoryRef = std::shared_ptr<TextureCubeFactory>;
};