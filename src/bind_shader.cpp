#include "bind_shader.h"

BindShader::BindShader(AbstractShaderRef shader, ShaderSettings settings)
{
	this->shader = shader;
	this->settings = settings;
}

void BindShader::execute()
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
