#include "buffer_es2.h"
#include "../util/log.h"
#include <optick.h>

Rendy::ES2::Buffer::Buffer(BufferTarget target, uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("target", to_string(target).c_str());
	OPTICK_TAG("size", size);

	this->target = target;
	set_data(size, ptr);
	generate_buffer();
}

Rendy::ES2::Buffer::~Buffer()
{
	OPTICK_EVENT();
	glDeleteBuffers(1, &id);
}

void Rendy::ES2::Buffer::reload()
{
	OPTICK_EVENT();
	if (!validate())
	{
		generate_buffer();
	}
}

bool Rendy::ES2::Buffer::validate() const
{
	OPTICK_EVENT();
	if (target == BufferTarget::Invalid)
	{
		return false;
	}

	if (!glIsBuffer(id))
	{
		return false;
	}

	return true;
}

void Rendy::ES2::Buffer::set_data(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	data = std::move(std::vector<uint8_t>(reinterpret_cast<const uint8_t*>(ptr), 
		reinterpret_cast<const uint8_t*>(ptr) + size));
}

void Rendy::ES2::Buffer::bind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	glBindBuffer(get_gl_target(), id);
}

void Rendy::ES2::Buffer::bind(uint32_t index)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("index", index);

	Log::error("You can't use glBindBufferBase at ES2!");
	assert(0);
}

void Rendy::ES2::Buffer::unbind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	glBindBuffer(get_gl_target(), 0);
}

void Rendy::ES2::Buffer::generate_buffer()
{
	OPTICK_EVENT();
	glGenBuffers(1, &id);
	OPTICK_TAG("id", id);
	bind();
	glBufferData(get_gl_target(), data.size(), &data[0], GL_STATIC_DRAW);
}

GLenum Rendy::ES2::Buffer::get_gl_target() const
{
	OPTICK_EVENT();
	return (target == BufferTarget::VBO) ? GL_ARRAY_BUFFER :
		GL_ELEMENT_ARRAY_BUFFER;
}

Rendy::BufferTarget Rendy::ES2::Buffer::get_target() const
{
	OPTICK_EVENT();
	return target;
}

uint32_t Rendy::ES2::Buffer::get_size() const
{
	OPTICK_EVENT();
	OPTICK_TAG("size", data.size());
	return static_cast<uint32_t>(data.size());
}
