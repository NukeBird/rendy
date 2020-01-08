#pragma once
#include "abstract_material.h"
#include "../texture/texture2d/abstract_texture2d.h"
#include "../texture/texture_cube/abstract_texture_cube.h"

namespace Rendy
{
	namespace ES3
	{
		class DefaultMaterial final : public AbstractMaterial
		{
		public:
			DefaultMaterial(ShaderFactoryRef shader_factory,
				AbstractTexture2DRef albedo_texture,
				AbstractTexture2DRef ambient_metallic_roughness_texture,
				AbstractTexture2DRef normal_texture,
				AbstractTextureCubeRef iem,
				AbstractTextureCubeRef pmrem,
				AbstractTexture2DRef lut,
				ShaderSourceRef source);
			virtual void reload() override;
			virtual void reload(ShaderSourceRef extra_source) override;
			virtual bool validate() const override;
			virtual bool validate(ShaderSourceRef extra_source) const override;
			virtual std::vector<CommandRef> to_command_list(const ShaderSettings& settings,
				ShaderSourceRef extra_source = nullptr) override;
			virtual bool uses_transparency() const override;
		private:
			AbstractTexture2DRef albedo_texture;
			AbstractTexture2DRef ambient_metallic_roughness_texture;
			AbstractTexture2DRef normal_texture; 
			AbstractTextureCubeRef iem;
			AbstractTextureCubeRef pmrem;
			AbstractTexture2DRef lut;
		};
	};
};