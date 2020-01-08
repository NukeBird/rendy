#pragma once
#include "buffer_es2.h"

namespace Rendy
{
	namespace ES3
	{
		class Buffer: public ES2::Buffer
		{
		public:
			Buffer(BufferTarget target, uint32_t size, const void* ptr);
			virtual void bind(uint32_t index) override;
		};
	};
};