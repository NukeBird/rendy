#pragma once
#include "buffer_es2.h"

namespace Rendy
{
	namespace ES3
	{
		class Buffer: public ES2::Buffer
		{
		public:
			virtual void bind(uint32_t index) override;
		};
	};
};