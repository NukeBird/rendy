#pragma once
#include "../abstract_engine.h"

namespace ES2
{
	class Engine: public AbstractEngine
	{
	public:
		virtual void push(BatchList draw_calls) override;
		virtual void push(CommandList commands) override;
		virtual void flush() override;
		virtual void reload() override;
	};
};