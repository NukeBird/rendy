#include "shader_source.h"
#include <cassert>

void Rendy::ShaderSource::set_source(ShaderType type, const std::string& source)
{
	sources[type] = source;
}

const std::string& Rendy::ShaderSource::get_source(ShaderType type)
{
	return sources[type];
}

Rendy::ShaderSourceRef Rendy::ShaderSource::combine(ShaderSourceRef source)
{
	assert(source != nullptr);
	return combine(*source);
}

Rendy::ShaderSourceRef Rendy::ShaderSource::combine(ShaderSource& source)
{
	auto result = std::make_shared<ShaderSource>();

	result->sources[ShaderType::VertexShader] = sources[ShaderType::VertexShader] +
		source.sources[ShaderType::VertexShader];

	result->sources[ShaderType::FragmentShader] = sources[ShaderType::FragmentShader] +
		source.sources[ShaderType::FragmentShader];

	return result;
}

bool Rendy::ShaderSource::operator==(ShaderSource& s)
{
	if (get_source(ShaderType::VertexShader) != s.get_source(ShaderType::VertexShader))
	{
		return false;
	}

	if (get_source(ShaderType::FragmentShader) != s.get_source(ShaderType::FragmentShader))
	{
		return false;
	}

	return true;
}
