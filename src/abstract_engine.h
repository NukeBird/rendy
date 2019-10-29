#pragma once
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "draw_call.h"
#include <vector>

using DrawableList = std::vector<AbstractDrawableRef>;
using CommandList = std::vector<CommandRef>;

struct AbstractEngine
{
	virtual void draw(DrawableList elements) = 0;
	virtual void draw(DrawCallList draw_calls) = 0;
	virtual void draw(CommandList commands) = 0;
	virtual void reload() = 0;
};