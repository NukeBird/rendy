#pragma once
#include "../abstract_engine.h"

namespace ES2
{
	class Engine: public AbstractEngine
	{
	public:
		virtual void push(DrawCallList draw_calls) override;
		virtual void push(CommandList commands) override;
		virtual void flush() override;
		virtual void reload() override;
	};
};