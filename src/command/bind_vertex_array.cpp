#include "bind_vertex_array.h"

Rendy::BindVertexArray::BindVertexArray(AbstractVertexArrayRef vao, ShaderVariantRef shader_variant)
{
	this->vao = vao;
	this->shader_variant = shader_variant;
}

void Rendy::BindVertexArray::execute()
{
	if (vao && shader_variant)
	{
		vao->bind(shader_variant);
	}
}
