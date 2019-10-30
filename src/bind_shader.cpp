#include "bind_shader.h"

Rendy::BindShader::BindShader(AbstractShaderRef shader, ShaderSettings settings)
{
	this->shader = shader;
	this->settings = settings;
}

void Rendy::BindShader::execute()
{
	if (shader)
	{
		auto shader_variant = shader->compile(settings);
		
		if (shader_variant)
		{
			shader_variant->bind();
		}
	}
}
