#include "default_material.h"
#include "shader_factory.h"

DefaultMaterial::DefaultMaterial(const AbstractTextureRef& diffuse_texture,
	const AbstractTextureRef& normal_texture)
{
	auto shader_manager = ShaderFactory::get_instance();
	shader = shader_manager->get_generic_shader();
	this->diffuse_texture = diffuse_texture;
	this->normal_texture = normal_texture;
}

void DefaultMaterial::reload()
{
	shader->reload();

	if (diffuse_texture)
	{
		diffuse_texture->reload();
	}

	if (normal_texture)
	{
		normal_texture->reload();
	}
}

bool DefaultMaterial::validate() const
{
	if (diffuse_texture)
	{
		if (!diffuse_texture->validate())
		{
			return false;
		}
	}

	if (normal_texture)
	{
		if (!normal_texture->validate())
		{
			return false;
		}
	}

	if (!shader->validate())
	{
		return false;
	}

	return true;
}

AbstractShaderRef DefaultMaterial::get_shader()
{
	return shader;
}

uint32_t DefaultMaterial::get_flags() const
{
	uint32_t flags = 0;
	
	if (diffuse_texture)
	{
		flags |= USE_COLOR_TEXTURE;
	}

	if (normal_texture)
	{
		flags |= USE_NORMAL_TEXTURE;
	}

	return flags;
}

void DefaultMaterial::bind(const ShaderSettings& settings)
{
	auto shader_variant = shader->compile(settings);
	shader_variant->bind();

	if (diffuse_texture)
	{
		diffuse_texture->bind(0); //TODO: get slot by texture name
	}

	if (normal_texture)
	{
		normal_texture->bind(1); //TODO: get slot by texture name
	}
}

void DefaultMaterial::unbind(uint32_t extra_flags)
{
	ShaderSettings settings; //TODO: lights
	settings.flags = get_flags() | extra_flags;
	auto shader_variant = shader->compile(settings);
	shader_variant->unbind();

	if (diffuse_texture)
	{
		diffuse_texture->unbind(0); //TODO: get slot by texture name
	}

	if (normal_texture)
	{
		normal_texture->unbind(1); //TODO: get slot by texture name
	}
}
