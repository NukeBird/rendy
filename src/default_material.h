#pragma once
#include "abstract_material.h"
#include "abstract_texture2d.h"
#include "abstract_shader.h"

namespace Rendy
{
	class DefaultMaterial final : public AbstractMaterial
	{
	public:
		DefaultMaterial(const AbstractTexture2DRef& diffuse_texture,
			const AbstractTexture2DRef& normal_texture = nullptr);
		virtual void reload() override;
		virtual bool validate() const override;
		virtual AbstractShaderRef get_shader() override;
		virtual ShaderVariantRef get_shader_variant(uint32_t extra_flags) override;
		virtual uint32_t get_flags() const override;
		virtual std::vector<CommandRef> to_command_list(uint32_t extra_flags) override;
		virtual void bind(const ShaderSettings& settings) override;
		virtual void unbind(uint32_t extra_flags) override;
	private:
		AbstractTexture2DRef diffuse_texture;
		AbstractTexture2DRef normal_texture;
		AbstractShaderRef shader;
	};
};