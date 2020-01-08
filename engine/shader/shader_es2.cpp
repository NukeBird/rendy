#include "shader_es2.h"
#include "shader_variant_es2.h"
#include "../common.h"
#include <optick.h>

Rendy::ES2::Shader::Shader(ShaderSourceRef source): AbstractShader(source)
{
	OPTICK_EVENT();
}

Rendy::ES2::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source):
	AbstractShader(vertex_source, fragment_source)
{
	OPTICK_EVENT();
}

std::string Rendy::ES2::Shader::generate_meta(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	std::string meta = 
		"#version 430\n" //TODO
		"precision highp float;\n" //TODO
		+ settings.generate_definitions();

	return meta;
}

Rendy::ShaderVariantRef Rendy::ES2::Shader::make_variant(const std::string& vertex_source, 
	const std::string& fragment_source)
{
	OPTICK_EVENT();
	return std::make_shared<ES2::ShaderVariant>(vertex_source, fragment_source);
}
