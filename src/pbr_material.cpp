#include "pbr_material.h"
#include "shader_factory.h"

PBRMaterial::PBRMaterial(const AbstractTextureRef& albedo_texture,
	const AbstractTextureRef& ambient_metallic_roughness_texture, 
	const AbstractTextureRef& normal_texture)
{
	auto shader_manager = ShaderFactory::get_instance();
	shader = shader_manager->get_generic_shader();

	this->albedo_texture = albedo_texture;
	this->ambient_metallic_roughness_texture = 
		ambient_metallic_roughness_texture;
	this->normal_texture = normal_texture;
}

void PBRMaterial::reload()
{
	shader->reload();

	if (albedo_texture)
	{
		albedo_texture->reload();
	}

	if (ambient_metallic_roughness_texture)
	{
		ambient_metallic_roughness_texture->reload();
	}

	if (normal_texture)
	{
		normal_texture->reload();
	}
}

bool PBRMaterial::validate() const
{
	if (!albedo_texture || !albedo_texture->validate())
	{
		return false;
	}

	if (!ambient_metallic_roughness_texture ||
		!ambient_metallic_roughness_texture->validate())
	{
		return false;
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

AbstractShaderRef PBRMaterial::get_shader()
{
	return shader;
}

uint32_t PBRMaterial::get_flags() const
{
	uint32_t flags = USE_COLOR_TEXTURE | USE_METALLIC_ROUGHNESS_TEXTURE;

	if (normal_texture)
	{
		flags |= USE_NORMAL_TEXTURE;
	}

	return flags;
}

void PBRMaterial::bind(uint32_t extra_flags)
{
	ShaderSettings settings; //TODO: lights
	settings.flags = get_flags() | extra_flags;
	auto shader_variant = shader->compile(settings);
	shader_variant->bind();

	if (albedo_texture)
	{
		shader_variant->set_uniform("color_texture", 0);
		albedo_texture->bind(0); //TODO: get slot by texture name
	}

	if (normal_texture)
	{
		shader_variant->set_uniform("normal_texture", 1);
		normal_texture->bind(1); //TODO: get slot by texture name
	}

	if (ambient_metallic_roughness_texture)
	{
		shader_variant->set_uniform("metallic_roughness_texture", 2);
		ambient_metallic_roughness_texture->bind(2); //TODO: get slot by texture name
	}
}

void PBRMaterial::unbind(uint32_t extra_flags)
{
	ShaderSettings settings; //TODO: lights
	settings.flags = get_flags() | extra_flags;
	auto shader_variant = shader->compile(settings);
	shader_variant->unbind();

	if (albedo_texture)
	{
		albedo_texture->unbind(0); //TODO: get slot by texture name
	}

	if (normal_texture)
	{
		normal_texture->unbind(1); //TODO: get slot by texture name
	}

	if (ambient_metallic_roughness_texture)
	{
		ambient_metallic_roughness_texture->unbind(2); //TODO: get slot by texture name
	}
}
