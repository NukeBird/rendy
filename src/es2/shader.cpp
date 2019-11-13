#include "shader.h"
#include "shader_variant.h"
#include "../common.h"
#include <optick.h>

Rendy::ES2::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source)
{
	OPTICK_EVENT();

	this->vertex_source = vertex_source;
	this->fragment_source = fragment_source;

	ShaderSettings settings;
	settings.flags = EVERYTHING;

	compile(settings);
}

void Rendy::ES2::Shader::reload()
{
	OPTICK_EVENT();

	for (auto& v : variants)
	{
		v.second->reload();
	}
}

Rendy::ShaderVariantRef Rendy::ES2::Shader::compile(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	auto it = variants.find(settings);

	if (it == variants.end())
	{
		const std::string meta = 
			"#version 330 core\n" //TODO
			"#define MAX_BONES 120\n"
			+ settings.generate_definitions();
		const std::string vertex_source_variant = meta + vertex_source;
		const std::string fragment_source_variant = meta + fragment_source;

		variants[settings] = std::make_shared<ShaderVariant>(vertex_source_variant,
			fragment_source_variant);
			
		it = variants.find(settings);
	}

	return it->second;
}

bool Rendy::ES2::Shader::validate() const
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

uint32_t Rendy::ES2::Shader::get_variant_count() const
{
	OPTICK_EVENT();

	return static_cast<uint32_t>(variants.size());
}
