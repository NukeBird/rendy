#pragma once
#include "abstract_material.h"
#include "abstract_texture.h"
#include "abstract_shader.h"

class DefaultMaterial final: public AbstractMaterial
{
public:
	DefaultMaterial(const AbstractTextureRef& diffuse_texture,
		const AbstractTextureRef& normal_texture = nullptr);
	virtual void reload() override;
	virtual bool validate() const override;
	virtual AbstractShaderRef get_shader() override;
	virtual uint32_t get_flags() const override;
	virtual void bind(uint32_t extra_flags) override;
	virtual void unbind(uint32_t extra_flags) override;
private:
	AbstractTextureRef diffuse_texture;
	AbstractTextureRef normal_texture;
	AbstractShaderRef shader;
};