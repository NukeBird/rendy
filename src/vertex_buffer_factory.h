#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <vector>

namespace Rendy
{
	class VertexBufferFactory : public Singleton<VertexBufferFactory>
	{
	public:
		AbstractBufferRef make(uint32_t size, const void* ptr);
	};
};