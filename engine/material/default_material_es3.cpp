#include "default_material_es3.h"
#include "../command/bind_shader.h"
#include "../command/bind_texture2d.h"
#include "../command/bind_texture_cube.h"
#include "../command/set_uniform.h"
#include "../util/log.h"
#include "../engine.h"
#include <optick.h>

Rendy::ES3::DefaultMaterial::DefaultMaterial(ShaderFactoryRef shader_factory,
	AbstractTexture2DRef albedo_texture,
	AbstractTexture2DRef ambient_metallic_roughness_texture, 
	AbstractTexture2DRef normal_texture,
	AbstractTextureCubeRef iem,
	AbstractTextureCubeRef pmrem,
	AbstractTexture2DRef lut,
	ShaderSourceRef source): AbstractMaterial(shader_factory)
{
	OPTICK_EVENT();

	this->albedo_texture = albedo_texture;
	this->ambient_metallic_roughness_texture = 
		ambient_metallic_roughness_texture;
	this->normal_texture = normal_texture;
	this->iem = iem;
	this->pmrem = pmrem;
	this->lut = lut;
	this->source = source;

	flags = 0;

	if (albedo_texture)
	{
		flags |= USE_COLOR_TEXTURE;
	}

	if (ambient_metallic_roughness_texture)
	{
		flags |= USE_METALLIC_ROUGHNESS_TEXTURE;
	}

	if (normal_texture)
	{
		flags |= USE_NORMAL_TEXTURE;
	}
}

void Rendy::ES3::DefaultMaterial::reload()
{
	OPTICK_EVENT();

	get_shader()->reload();

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

void Rendy::ES3::DefaultMaterial::reload(ShaderSourceRef extra_source)
{
	OPTICK_EVENT();

	get_shader(extra_source)->reload();

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

bool Rendy::ES3::DefaultMaterial::validate() const
{
	OPTICK_EVENT();

	if (albedo_texture)
	{
		if(!albedo_texture->validate())
		{
			Log::info("Invalid albedo texture\n");
			return false;
		}
	}

	if (ambient_metallic_roughness_texture)
	{
		if (!ambient_metallic_roughness_texture->validate())
		{
			Log::info("Invalid ambient metallic roughness texture\n");
			return false;
		}
	}

	if (normal_texture)
	{
		if (!normal_texture->validate())
		{
			Log::info("Invalid normal texture\n");
			return false;
		}
	}

	if (!get_shader()->validate())
	{
		Log::info("Invalid shader\n");
		return false;
	}

	return true;
}

bool Rendy::ES3::DefaultMaterial::validate(ShaderSourceRef extra_source) const
{
	OPTICK_EVENT();

	if (albedo_texture)
	{
		if (!albedo_texture->validate())
		{
			Log::info("Invalid albedo texture\n");
			return false;
		}
	}

	if (ambient_metallic_roughness_texture)
	{
		if (!ambient_metallic_roughness_texture->validate())
		{
			Log::info("Invalid ambient metallic roughness texture\n");
			return false;
		}
	}

	if (normal_texture)
	{
		if (!normal_texture->validate())
		{
			Log::info("Invalid normal texture\n");
			return false;
		}
	}

	if (!get_shader(extra_source)->validate())
	{
		Log::info("Invalid shader\n");
		return false;
	}

	return true;
}

#include "../util/log.h"

std::vector<Rendy::CommandRef> Rendy::ES3::DefaultMaterial::to_command_list(const ShaderSettings& settings,
	ShaderSourceRef extra_source)
{
	OPTICK_EVENT();

	auto shader_variant = get_shader_variant(settings, extra_source);
	auto processed_settings = process_settings(settings);

	std::vector<CommandRef> list;

	list.emplace_back(std::make_shared<BindShader>(get_shader(extra_source), processed_settings));

	if (processed_settings.flags & USE_COLOR_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"color_texture", 0));
		list.emplace_back(std::make_shared<BindTexture2D>(albedo_texture, 0));
	}

	if (processed_settings.flags & USE_NORMAL_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"normal_texture", 1));
		list.emplace_back(std::make_shared<BindTexture2D>(normal_texture, 1));
	}

	if (processed_settings.flags & USE_METALLIC_ROUGHNESS_TEXTURE)
	{
		list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
			"metallic_roughness_texture", 2));
		list.emplace_back(std::make_shared<BindTexture2D>(
			ambient_metallic_roughness_texture, 2));
	}

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"iem", 3));
	list.emplace_back(std::make_shared<BindTextureCube>(iem, 3));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"pmrem", 4));
	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"u_max_pmrem_level", static_cast<int>(pmrem->get_max_level())));
	list.emplace_back(std::make_shared<BindTextureCube>(pmrem, 4));

	list.emplace_back(std::make_shared<SetUniform<int>>(shader_variant,
		"lut", 5));
	list.emplace_back(std::make_shared<BindTexture2D>(lut, 5));

	return list;
}

bool Rendy::ES3::DefaultMaterial::uses_transparency() const
{
	if (albedo_texture)
	{
		return albedo_texture->uses_transparency();
	}

	return false;
}