#include "vertex_buffer_factory.h"
#include "es2/buffer.h"
#include <algorithm>

AbstractBufferRef VertexBufferFactory::make(uint32_t size, const void* ptr)
{
	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto vbo = std::make_shared<ES2::Buffer>(BufferTarget::VBO, size, ptr);
	return vbo;
}