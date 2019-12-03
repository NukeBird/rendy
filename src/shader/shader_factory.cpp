#include "shader_factory.h"
#include "shader_es2.h"
#include "shader_es3.h"
#include <optick.h>

Rendy::ShaderFactory::ShaderFactory(OGL version)
{
	OPTICK_EVENT();
	this->version = version;
}

Rendy::AbstractShaderRef Rendy::ShaderFactory::make(const std::string& vertex_source,
	const std::string& fragment_source)
{
	OPTICK_EVENT();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::Shader>(vertex_source, fragment_source);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::Shader>(vertex_source, fragment_source);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::Shader>(vertex_source, fragment_source);
}