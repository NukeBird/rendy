#pragma once
#include "abstract_vertex_array.h"
#include "abstract_command.h"

class Draw: public AbstractCommand
{
public:
	Draw(AbstractVertexArrayRef vao);
	virtual void execute() override;
private:
	AbstractVertexArrayRef vao;
};

using DrawRef = std::shared_ptr<Draw>;