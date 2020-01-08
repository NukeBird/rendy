#pragma once
#include "../vertex_array/abstract_vertex_array.h"
#include "abstract_command.h"

namespace Rendy
{
	class Draw : public AbstractCommand
	{
	public:
		Draw(AbstractVertexArrayRef vao);
		virtual void execute() override;
	private:
		AbstractVertexArrayRef vao;
	};

	using DrawRef = std::shared_ptr<Draw>;
};