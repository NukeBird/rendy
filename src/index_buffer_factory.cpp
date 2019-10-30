#include "index_buffer_factory.h"
#include "es2/buffer.h"
#include <algorithm>
#include <cassert>
#include <optick.h>

Rendy::AbstractBufferRef Rendy::IndexBufferFactory::make(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES2::Buffer>(BufferTarget::IBO, size, ptr);
	return ibo;
}