#pragma once
#include "abstract_buffer.h"
#include "abstract_command_call.h"

class BindBuffer: public AbstractCommandCall
{
public:
	BindBuffer(AbstractBufferRef buffer);
	virtual void execute() override;
private:
	AbstractBufferRef buffer;
};