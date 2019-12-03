#pragma once
#include "../ogl.hpp"
#include "../image/image_set.h"
#include "../texture/texture_cube/abstract_texture_cube.h"
#include "../texture/texture2d/texture2d_factory.h"
#include "../shader/abstract_shader.h"
#include "../shader/shader_factory.h"
#include "abstract_material.h"
#include <memory>
#include <string>

namespace Rendy
{
	class MaterialFactory final
	{
	public:
		MaterialFactory(OGL version, Texture2DFactoryRef texture_factory, 
			ShaderFactoryRef shader_factory, AbstractTextureCubeRef iem,
			AbstractTextureCubeRef pmrem = nullptr, 
			AbstractTexture2DRef lut = nullptr);
		AbstractMaterialRef make(ImageSetRef image_set);
	private:
		void load_shader(ShaderFactoryRef shader_factory);
		AbstractMaterialRef make_es2(ImageSetRef image_set);
		AbstractMaterialRef make_es3(ImageSetRef image_set);

		Texture2DFactoryRef texture_factory;
		AbstractTexture2DRef lut;
		AbstractShaderRef shader;
		AbstractTextureCubeRef iem;
		AbstractTextureCubeRef pmrem;
		OGL version;
	};

	using MaterialFactoryRef = std::shared_ptr<MaterialFactory>;
};