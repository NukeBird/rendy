#pragma once
#include "abstract_material.h"
#include "../texture/texture2d/abstract_texture2d.h"
#include "../texture/texture_cube/abstract_texture_cube.h"

namespace Rendy
{
	namespace ES2
	{
		class DefaultMaterial final : public AbstractMaterial
		{
		public:
			DefaultMaterial(AbstractTexture2DRef diffuse_texture,
				AbstractTexture2DRef normal_texture,
				AbstractTextureCubeRef iem_texture,
				AbstractShaderRef shader);
			virtual void reload() override;
			virtual bool validate() const override;
			virtual AbstractShaderRef get_shader() override;
			virtual uint32_t get_flags() const override;
			virtual std::vector<CommandRef> to_command_list(const ShaderSettings& settings) override;
			virtual void bind(const ShaderSettings& settings) override;
			virtual void unbind(const ShaderSettings& settings) override;
		private:
			AbstractTexture2DRef diffuse_texture;
			AbstractTexture2DRef normal_texture;
			AbstractTextureCubeRef iem_texture;
			AbstractShaderRef shader;
		};
	};
};