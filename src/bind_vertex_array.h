#pragma once
#include "vertex_array/abstract_vertex_array.h"
#include "abstract_command.h"

namespace Rendy
{
	class BindVertexArray : public AbstractCommand
	{
	public:
		BindVertexArray(AbstractVertexArrayRef vao, ShaderVariantRef shader_variant);
		virtual void execute() override;
	private:
		AbstractVertexArrayRef vao;
		ShaderVariantRef shader_variant;
	};

	using BindVertexArrayRef = std::shared_ptr<BindVertexArray>;
};