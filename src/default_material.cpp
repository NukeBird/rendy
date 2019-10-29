#include "default_material.h"
#include "shader_factory.h"
#include "bind_shader.h"
#include "bind_texture2d.h"
#include "set_uniform.h"
#include <optick.h>

DefaultMaterial::DefaultMaterial(const AbstractTexture2DRef& diffuse_texture,
	const AbstractTexture2DRef& normal_texture)
{
	OPTICK_EVENT();

	auto shader_manager = ShaderFactory::get_instance();
	shader = shader_manager->get_generic_shader();
	this->diffuse_texture = diffuse_texture;
	this->normal_texture = normal_texture;
}

void DefaultMaterial::reload()
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

bool DefaultMaterial::validate() const
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

AbstractShaderRef DefaultMaterial::get_shader()
{
	OPTICK_EVENT();
	return shader;
}

uint32_t DefaultMaterial::get_flags() const
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

std::vector<CommandRef> DefaultMaterial::to_command_list(uint32_t extra_flags)
{
	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();

	auto shader_variant = shader->compile(settings);

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(shader, settings));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"color_texture", 0));
	list.emplace_back(std::make_shared<BindTexture2D>(diffuse_texture, 0));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"normal_texture", 1));
	list.emplace_back(std::make_shared<BindTexture2D>(normal_texture, 1));

	return list;
}

void DefaultMaterial::bind(const ShaderSettings& settings)
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

void DefaultMaterial::unbind(uint32_t extra_flags)
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
