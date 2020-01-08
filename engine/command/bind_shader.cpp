#include "bind_shader.h"
#include <optick.h>

Rendy::BindShader::BindShader(AbstractShaderRef shader, ShaderSettings settings)
{
	OPTICK_EVENT();

	this->shader = shader;
	this->settings = settings;
}

void Rendy::BindShader::execute()
{
	OPTICK_EVENT();

	if (shader)
	{
		auto shader_variant = shader->compile(settings);
		
		if (shader_variant)
		{
			shader_variant->bind();
		}
	}
}
