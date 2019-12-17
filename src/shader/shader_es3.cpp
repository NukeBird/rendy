#include "shader_es3.h"
#include "shader_variant_es3.h"
#include "../common.h"
#include <optick.h>

Rendy::ES3::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source) :
	AbstractShader(vertex_source, fragment_source)
{
	OPTICK_EVENT();
}

std::string Rendy::ES3::Shader::generate_meta(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	std::string meta =
		"#version 430\n" //TODO
		+ settings.generate_definitions();

	return meta;
}

Rendy::ShaderVariantRef Rendy::ES3::Shader::make_variant(const std::string& vertex_source,
	const std::string& fragment_source)
{
	OPTICK_EVENT();
	return std::make_shared<ES3::ShaderVariant>(vertex_source, fragment_source);
}
