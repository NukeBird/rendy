#pragma once
#include "../abstract_engine.h"

namespace ES2
{
	class Engine: public AbstractEngine
	{
	public:
		virtual void draw(DrawCallList draw_calls) override;
		virtual void draw(CommandList commands) override;
		virtual void reload() override;
	};
};