#include "bind_vertex_array.h"
#include <optick.h>

Rendy::BindVertexArray::BindVertexArray(AbstractVertexArrayRef vao, ShaderVariantRef shader_variant)
{
	OPTICK_EVENT();

	this->vao = vao;
	this->shader_variant = shader_variant;
}

void Rendy::BindVertexArray::execute()
{
	OPTICK_EVENT();

	if (vao && shader_variant)
	{
		vao->bind(shader_variant);
	}
}
