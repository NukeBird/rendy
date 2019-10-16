#include "shader_factory.h"
#include "es2/shader.h"
#include <algorithm>
#include "generic_shader_sources.hpp"

AbstractShaderRef ShaderFactory::make(const std::string& vtx, const std::string& frg)
{
	auto shader = std::make_shared<ES2::Shader>(vtx, frg);
	return shader;
}

AbstractShaderRef ShaderFactory::get_generic_shader()
{
	if (!generic_shader)
	{
		generic_shader = make(generic_vertex_shader, generic_fragment_shader); //TODO
	}

	return generic_shader;
}