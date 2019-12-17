#include "shader_es3.h"
#include "shader_variant_es3.h"
#include "../common.h"
#include <optick.h>

Rendy::ES3::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source)
{
	OPTICK_EVENT();

	this->vertex_source = vertex_source;
	this->fragment_source = fragment_source;

	ShaderSettings settings;
	settings.flags = EVERYTHING;
	settings.custom_definitions["MAX_BONES"] = 120;

	compile(settings);
}

void Rendy::ES3::Shader::reload()
{
	OPTICK_EVENT();

	for (auto& v : variants)
	{
		v.second->reload();
	}
}

Rendy::ShaderVariantRef Rendy::ES3::Shader::compile(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	auto it = variants.find(settings);

	if (it == variants.end())
	{
		const std::string meta = 
			"#version 430\n" //TODO
			+ settings.generate_definitions();
		const std::string vertex_source_variant = meta + vertex_source;
		const std::string fragment_source_variant = meta + fragment_source;

		variants[settings] = std::make_shared<ES3::ShaderVariant>(vertex_source_variant,
			fragment_source_variant);
			
		it = variants.find(settings);
	}

	return it->second;
}

bool Rendy::ES3::Shader::validate() const
{
	OPTICK_EVENT();

	for (auto& v : variants)
	{
		if (!v.second->validate())
		{
			return false;
		}
	}

	return true;
}

uint32_t Rendy::ES3::Shader::get_variant_count() const
{
	OPTICK_EVENT();

	return static_cast<uint32_t>(variants.size());
}