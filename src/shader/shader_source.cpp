#include "shader_source.h"
#include <optick.h>
#include <cassert>

Rendy::ShaderSource::ShaderSource(const std::string& vtx, const std::string& frg)
{
	OPTICK_EVENT();

	set_source(ShaderType::VertexShader, vtx);
	set_source(ShaderType::FragmentShader, frg);
}

void Rendy::ShaderSource::set_source(ShaderType type, const std::string& source)
{
	OPTICK_EVENT();
	sources[type] = source;
}

const std::string& Rendy::ShaderSource::get_source(ShaderType type)
{
	OPTICK_EVENT();
	return sources[type];
}

Rendy::ShaderSourceRef Rendy::ShaderSource::combine(ShaderSourceRef source)
{
	OPTICK_EVENT();
	assert(source != nullptr);
	return combine(*source);
}

Rendy::ShaderSourceRef Rendy::ShaderSource::combine(ShaderSource& source)
{
	OPTICK_EVENT();

	auto result = std::make_shared<ShaderSource>();

	result->sources[ShaderType::VertexShader] = sources[ShaderType::VertexShader] +
		source.sources[ShaderType::VertexShader];

	result->sources[ShaderType::FragmentShader] = sources[ShaderType::FragmentShader] +
		source.sources[ShaderType::FragmentShader];

	return result;
}

bool Rendy::ShaderSource::operator==(ShaderSource& s)
{
	OPTICK_EVENT();

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
