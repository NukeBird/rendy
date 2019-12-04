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
			DefaultMaterial(AbstractTexture2DRef albedo_texture,
				AbstractTexture2DRef ambient_metallic_roughness_texture,
				AbstractTexture2DRef normal_texture,
				AbstractTextureCubeRef iem,
				AbstractTextureCubeRef pmrem,
				AbstractTexture2DRef lut,
				AbstractShaderRef shader);
			virtual void reload() override;
			virtual bool validate() const override;
			virtual AbstractShaderRef get_shader() override;
			virtual uint32_t get_flags() const override;
			virtual std::vector<CommandRef> to_command_list(uint32_t extra_flags) override;
			virtual void bind(const ShaderSettings& settings) override;
			virtual void unbind(uint32_t extra_flags) override;
		private:
			AbstractTexture2DRef albedo_texture;
			AbstractTexture2DRef ambient_metallic_roughness_texture;
			AbstractTexture2DRef normal_texture; 
			AbstractTextureCubeRef iem;
			AbstractTextureCubeRef pmrem;
			AbstractTexture2DRef lut;
			AbstractShaderRef shader;
		};
	};
};