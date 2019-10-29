#pragma once
#include "abstract_material.h"
#include "abstract_texture2d.h"
#include "abstract_shader.h"

class PBRMaterial final: public AbstractMaterial
{
public:
	PBRMaterial(const AbstractTexture2DRef& albedo_texture,
		const AbstractTexture2DRef& ambient_metallic_roughness_texture,
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
	AbstractTexture2DRef albedo_texture;
	AbstractTexture2DRef ambient_metallic_roughness_texture;
	AbstractTexture2DRef normal_texture;
	AbstractShaderRef shader;
};