#pragma once
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "batch.h"
#include <vector>

using CommandList = std::vector<CommandRef>;

struct AbstractEngine
{
	virtual void push(BatchList draw_calls) = 0;
	virtual void push(CommandList commands) = 0;
	virtual void flush() = 0;
	virtual void reload() = 0;
};