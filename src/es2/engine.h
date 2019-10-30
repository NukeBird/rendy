#pragma once
#include "../abstract_engine.h"

namespace Rendy
{
	namespace ES2
	{
		class Engine final: public AbstractEngine
		{
		public:
			virtual void push(BatchList batches) override;
			virtual void push(CommandList commands) override;
			virtual void flush() override;
			virtual void reload() override;

			virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) override;
			virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) override;
		};
	};
};