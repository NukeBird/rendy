#pragma once
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "draw_call.h"
#include <vector>

using CommandList = std::vector<CommandRef>;

struct AbstractEngine
{
	virtual void draw(DrawCallList draw_calls) = 0;
	virtual void draw(CommandList commands) = 0;
	virtual void reload() = 0;
};