#include "pbr_material.h"
#include "../bind_shader.h"
#include "../bind_texture2d.h"
#include "../bind_texture_cube.h"
#include "../set_uniform.h"
#include <optick.h>

Rendy::ES3::PBRMaterial::PBRMaterial(AbstractTexture2DRef albedo_texture,
	AbstractTexture2DRef ambient_metallic_roughness_texture, 
	AbstractTexture2DRef normal_texture,
	AbstractTextureCubeRef iem,
	AbstractShaderRef shader)
{
	OPTICK_EVENT();

	this->albedo_texture = albedo_texture;
	this->ambient_metallic_roughness_texture = 
		ambient_metallic_roughness_texture;
	this->normal_texture = normal_texture;
	this->iem = iem;
	this->shader = shader;
}

void Rendy::ES3::PBRMaterial::reload()
{
	OPTICK_EVENT();

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

bool Rendy::ES3::PBRMaterial::validate() const
{
	OPTICK_EVENT();

	if (!albedo_texture || !albedo_texture->validate())
	{
		printf("Invalid albedo texture\n");
		return false;
	}

	if (!ambient_metallic_roughness_texture ||
		!ambient_metallic_roughness_texture->validate())
	{
		printf("Invalid ambient metallic roughness texture\n");
		return false;
	}

	if (normal_texture)
	{
		if (!normal_texture->validate())
		{
			printf("Invalid normal texture\n");
			return false;
		}
	}

	if (!shader->validate())
	{
		printf("Invalid shader\n");
		return false;
	}

	return true;
}

Rendy::AbstractShaderRef Rendy::ES3::PBRMaterial::get_shader()
{
	OPTICK_EVENT();
	return shader;
}

Rendy::ShaderVariantRef Rendy::ES3::PBRMaterial::get_shader_variant(uint32_t extra_flags)
{
	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();
	return shader->compile(settings);
}

uint32_t Rendy::ES3::PBRMaterial::get_flags() const
{
	OPTICK_EVENT();

	uint32_t flags = USE_COLOR_TEXTURE | USE_METALLIC_ROUGHNESS_TEXTURE;

	if (normal_texture)
	{
		flags |= USE_NORMAL_TEXTURE;
	}

	return flags;
}

std::vector<Rendy::CommandRef> Rendy::ES3::PBRMaterial::to_command_list(uint32_t extra_flags)
{
	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();

	auto shader_variant = shader->compile(settings);

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(shader, settings));

	if (settings.flags & USE_COLOR_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"color_texture", 0));
		list.emplace_back(std::make_shared<BindTexture2D>(albedo_texture, 0));
	}

	if (settings.flags & USE_NORMAL_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"normal_texture", 1));
		list.emplace_back(std::make_shared<BindTexture2D>(normal_texture, 1));
	}

	if (settings.flags & USE_METALLIC_ROUGHNESS_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"metallic_roughness_texture", 2));
		list.emplace_back(std::make_shared<BindTexture2D>(
			ambient_metallic_roughness_texture, 2));
	}

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"u_iem", 6));
	list.emplace_back(std::make_shared<BindTextureCube>(iem, 6));

	return list;
}

void Rendy::ES3::PBRMaterial::bind(const ShaderSettings& settings)
{
	OPTICK_EVENT();

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

void Rendy::ES3::PBRMaterial::unbind(uint32_t extra_flags)
{
	OPTICK_EVENT();

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
