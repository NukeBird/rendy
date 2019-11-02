#include "default_material.h"
#include "../bind_shader.h"
#include "../bind_texture2d.h"
#include "../bind_texture_cube.h"
#include "../set_uniform.h"
#include <optick.h>

Rendy::ES2::DefaultMaterial::DefaultMaterial(AbstractTexture2DRef diffuse_texture,
	AbstractTexture2DRef normal_texture, AbstractTextureCubeRef iem_texture, 
	AbstractShaderRef shader)
{
	OPTICK_EVENT();

	this->diffuse_texture = diffuse_texture;
	this->normal_texture = normal_texture;
	this->iem_texture = iem_texture;
	this->shader = shader;
}

void Rendy::ES2::DefaultMaterial::reload()
{
	OPTICK_EVENT();

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

bool Rendy::ES2::DefaultMaterial::validate() const
{
	OPTICK_EVENT();

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

Rendy::AbstractShaderRef Rendy::ES2::DefaultMaterial::get_shader()
{
	OPTICK_EVENT();
	return shader;
}

Rendy::ShaderVariantRef Rendy::ES2::DefaultMaterial::get_shader_variant(uint32_t extra_flags)
{
	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();
	return shader->compile(settings);
}

uint32_t Rendy::ES2::DefaultMaterial::get_flags() const
{
	OPTICK_EVENT();

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

std::vector<Rendy::CommandRef> Rendy::ES2::DefaultMaterial::to_command_list(uint32_t extra_flags)
{
	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();

	auto shader_variant = get_shader_variant(extra_flags);

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(shader, settings));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"color_texture", 0));
	list.emplace_back(std::make_shared<BindTexture2D>(diffuse_texture, 0));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"normal_texture", 1));
	list.emplace_back(std::make_shared<BindTexture2D>(normal_texture, 1));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"u_iem", 2));
	list.emplace_back(std::make_shared<BindTextureCube>(iem_texture, 2));

	return list;
}

void Rendy::ES2::DefaultMaterial::bind(const ShaderSettings& settings)
{
	OPTICK_EVENT();

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

void Rendy::ES2::DefaultMaterial::unbind(uint32_t extra_flags)
{
	OPTICK_EVENT();

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
