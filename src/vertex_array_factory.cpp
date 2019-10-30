#include "vertex_array_factory.h"
#include "es2/vertex_array.h"
#include "vertex_buffer_factory.h"
#include "index_buffer_factory.h"
#include <algorithm>
#include <memory>
#include <optick.h>

Rendy::AbstractVertexArrayRef Rendy::VertexArrayFactory::make(const std::vector<float>& vbo_src,
	const std::vector<uint16_t>& ibo_src, const BufferLayoutRef & layout)
{
	OPTICK_EVENT();

	const uint32_t vbo_size = static_cast<uint32_t>(vbo_src.size() * sizeof(float));
	const void* vbo_ptr = &vbo_src[0];

	const uint32_t ibo_size = static_cast<uint32_t>(ibo_src.size() * sizeof(uint16_t));
	const void* ibo_ptr = &ibo_src[0];

	OPTICK_TAG("VBO size", vbo_size);
	OPTICK_TAG("IBO size", ibo_size);

	return make(vbo_ptr, vbo_size, ibo_ptr, ibo_size, layout);
}

Rendy::AbstractVertexArrayRef Rendy::VertexArrayFactory::make(const void* vbo_ptr, uint32_t vbo_size,
	const void* ibo_ptr, uint32_t ibo_size, const BufferLayoutRef& layout)
{
	OPTICK_EVENT();
	return std::make_shared<ES2::VertexArray>(
		VertexBufferFactory::get_instance()->make(vbo_size, vbo_ptr),
		IndexBufferFactory::get_instance()->make(ibo_size, ibo_ptr), layout);
}