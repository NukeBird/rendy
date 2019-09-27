#include "buffer.h"

ES2::Buffer::Buffer(BufferTarget target, uint32_t size, const void* ptr)
{
	this->target = target;
	set_data(size, ptr);
	generate_buffer();
}

ES2::Buffer::~Buffer()
{
	glDeleteBuffers(1, &id);
}

void ES2::Buffer::reload()
{
	if (!validate())
	{
		generate_buffer();
	}
}

bool ES2::Buffer::validate() const
{
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

void ES2::Buffer::set_data(uint32_t size, const void* ptr)
{
	data = std::move(std::vector<uint8_t>(reinterpret_cast<const uint8_t*>(ptr), 
		reinterpret_cast<const uint8_t*>(ptr) + size));
}

void ES2::Buffer::bind()
{
	glBindBuffer(get_gl_target(), id);
}

void ES2::Buffer::unbind()
{
	glBindBuffer(get_gl_target(), 0);
}

void ES2::Buffer::generate_buffer()
{
	glGenBuffers(1, &id);
	bind();
	glBufferData(get_gl_target(), data.size(), &data[0], GL_STATIC_DRAW);
}

GLenum ES2::Buffer::get_gl_target() const
{
	return (target == BufferTarget::VBO) ? GL_ARRAY_BUFFER :
		GL_ELEMENT_ARRAY_BUFFER;
}

BufferTarget ES2::Buffer::get_target() const
{
	return target;
}

uint32_t ES2::Buffer::get_size() const
{
	return static_cast<uint32_t>(data.size());
}
