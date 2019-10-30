#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <memory>
#include <vector>

namespace Rendy
{
	class IndexBufferFactory : public Singleton<IndexBufferFactory>
	{
	public:
		AbstractBufferRef make(uint32_t size, const void* ptr);
	};
};