#include "bind_vertex_array.h"

BindVertexArray::BindVertexArray(AbstractVertexArrayRef vao, ShaderVariantRef shader_variant)
{
	this->vao = vao;
	this->shader_variant = shader_variant;
}

void BindVertexArray::execute()
{
	if (vao && shader_variant)
	{
		vao->bind(shader_variant);
	}
}
