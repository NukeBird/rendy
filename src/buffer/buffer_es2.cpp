#include "buffer_es2.h"
#include "../util/log.h"
#include <optick.h>

Rendy::ES2::Buffer::Buffer(BufferTarget target, uint32_t size, 
	const void* ptr): AbstractBuffer(target, size, ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("target", to_string(target).c_str());
	OPTICK_TAG("size", size);

	assert(target != BufferTarget::SBO);
	assert(target != BufferTarget::CBO);
}

void Rendy::ES2::Buffer::bind(uint32_t index)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("index", index);

	Log::error("You can't use glBindBufferBase in ES2!");
	assert(0);
}