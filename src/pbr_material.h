#pragma once
#include "abstract_material.h"
#include "abstract_texture2d.h"
#include "abstract_shader.h"

class PBRMaterial final: public AbstractMaterial
{
public:
	PBRMaterial(const AbstractTextureRef& albedo_texture,
		const AbstractTextureRef& ambient_metallic_roughness_texture,
		const AbstractTextureRef& normal_texture = nullptr);
	virtual void reload() override;
	virtual bool validate() const override;
	virtual AbstractShaderRef get_shader() override;
	virtual uint32_t get_flags() const override;
	virtual void bind(const ShaderSettings& settings) override;
	virtual void unbind(uint32_t extra_flags) override;
private:
	AbstractTextureRef albedo_texture;
	AbstractTextureRef ambient_metallic_roughness_texture;
	AbstractTextureRef normal_texture;
	AbstractShaderRef shader;
};