#include "buffer_es3.h"
#include <optick.h>

Rendy::ES3::Buffer::Buffer(BufferTarget target, uint32_t size, 
	const void* ptr): ES2::Buffer(target, size, ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("target", to_string(target).c_str());
	OPTICK_TAG("size", size);
}

void Rendy::ES3::Buffer::bind(uint32_t index)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("index", index);

	assert(get_target() == BufferTarget::SBO || get_target() == BufferTarget::CBO);

	glBindBufferBase(get_gl_target(), index, id);
}