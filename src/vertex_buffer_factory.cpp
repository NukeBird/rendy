#include "vertex_buffer_factory.h"
#include "es2/buffer.h"
#include <algorithm>
#include <optick.h>

Rendy::AbstractBufferRef Rendy::VertexBufferFactory::make(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	OPTICK_TAG("size", size);

	auto vbo = std::make_shared<ES2::Buffer>(BufferTarget::VBO, size, ptr);
	return vbo;
}