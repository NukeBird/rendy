#include "default_material_es2.h"
#include "../command/bind_shader.h"
#include "../command/bind_texture2d.h"
#include "../command/bind_texture_cube.h"
#include "../command/set_uniform.h"
#include "../engine.h"
#include <optick.h>

Rendy::ES2::DefaultMaterial::DefaultMaterial(ShaderFactoryRef shader_factory,
	AbstractTexture2DRef diffuse_texture, AbstractTexture2DRef normal_texture, 
	AbstractTextureCubeRef iem_texture, ShaderSourceRef source): AbstractMaterial(shader_factory)
{
	OPTICK_EVENT();

	this->diffuse_texture = diffuse_texture;
	this->normal_texture = normal_texture;
	this->iem_texture = iem_texture;
	this->source = source;

	flags = 0;

	if (diffuse_texture)
	{
		flags |= USE_COLOR_TEXTURE;
	}

	if (normal_texture)
	{
		flags |= USE_NORMAL_TEXTURE;
	}
}

void Rendy::ES2::DefaultMaterial::reload()
{
	OPTICK_EVENT();
	reload(nullptr);
}

void Rendy::ES2::DefaultMaterial::reload(ShaderSourceRef extra_source)
{
	OPTICK_EVENT();

	get_shader(extra_source)->reload();

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
	return validate(nullptr);
}

bool Rendy::ES2::DefaultMaterial::validate(ShaderSourceRef extra_source) const
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

	if (!get_shader(source)->validate())
	{
		return false;
	}

	return true;
}

std::vector<Rendy::CommandRef> Rendy::ES2::DefaultMaterial::to_command_list(const ShaderSettings& settings,
	ShaderSourceRef extra_source)
{
	OPTICK_EVENT();

	auto shader_variant = get_shader_variant(settings, extra_source);
	auto processed_settings = process_settings(settings);

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(get_shader(extra_source), processed_settings));

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

bool Rendy::ES2::DefaultMaterial::uses_transparency() const
{
	if (diffuse_texture)
	{
		return diffuse_texture->uses_transparency();
	}

	return false;
}