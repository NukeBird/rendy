#include "index_buffer_factory.h"
#include "es2/buffer.h"
#include <algorithm>
#include <cassert>

AbstractBufferRef IndexBufferFactory::make(uint32_t size, const void* ptr)
{
	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES2::Buffer>(BufferTarget::IBO, size, ptr);
	return ibo;
}