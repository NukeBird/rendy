#include "shader_factory.h"
#include "shader_es2.h"
#include "shader_es3.h"
#include "../util/log.h"
#include <optick.h>

Rendy::ShaderFactory::ShaderFactory(OGL version)
{
	OPTICK_EVENT();
	this->version = version;
}

Rendy::AbstractShaderRef Rendy::ShaderFactory::make(ShaderSourceRef source)
{
	OPTICK_EVENT();

	auto shader = find_in_cache(source);

	if (!shader)
	{
		switch (version)
		{
			case Rendy::OGL::ES20:
			{
				shader = std::make_shared<ES2::Shader>(source);
				break;
			}
			case Rendy::OGL::ES31:
			{
				shader = std::make_shared<ES3::Shader>(source);
				break;
			}
		}

		shader_map[source] = shader;
	}

	//TODO: choose best
	return shader;
}

Rendy::AbstractShaderRef Rendy::ShaderFactory::make(const std::string& vtx, const std::string& frg)
{
	auto source = std::make_shared<ShaderSource>(vtx, frg);
	return make(source);
}

Rendy::AbstractShaderRef Rendy::ShaderFactory::find_in_cache(ShaderSourceRef source)
{
	auto it = std::find_if(shader_map.begin(), shader_map.end(), 
		[source](const std::pair<ShaderSourceRef, AbstractShaderRef>& v)
		{
			return v.first->operator==(*source);
		});	

	if (it != shader_map.end())
	{
		return it->second;
	}

	return nullptr;
}