#pragma once
#include "abstract_vertex_array.h"
#include "abstract_command_call.h"

class BindVertexArray: public AbstractCommandCall
{
public:
	BindVertexArray(AbstractVertexArrayRef vao, ShaderVariantRef shader_variant);
	virtual void execute() override;
private:
	AbstractVertexArrayRef vao;
	ShaderVariantRef shader_variant;
};

using BindBufferRef = std::shared_ptr<BindBufferRef>;