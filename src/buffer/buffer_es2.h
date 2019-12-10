#pragma once
#include "abstract_buffer.h"
#include <vector>
#include "../common.h"

namespace Rendy
{
	namespace ES2
	{
		class Buffer: public AbstractBuffer
		{
		public:
			Buffer(BufferTarget target, uint32_t size, const void* ptr);
			virtual void bind(uint32_t index) override;
		};
	};
};