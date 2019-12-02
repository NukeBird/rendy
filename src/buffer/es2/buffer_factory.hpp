#pragma once
#include "../abstract_buffer_factory.h"
#include "buffer.h"

namespace Rendy
{
	namespace ES2
	{
		class BufferFactory final: public AbstractBufferFactory
		{
			virtual AbstractBufferRef make(BufferTarget target, uint32_t size,
				const void* ptr) override
			{
				return std::shared_ptr<ES2::Buffer>(target, size, ptr);
			}
		};
	};
};