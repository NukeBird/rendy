#include "abstract_buffer.h"
#include <optick.h>

std::string Rendy::to_string(BufferTarget target)
{
	OPTICK_EVENT();

	switch(target)
	{
		case BufferTarget::VBO:
		{
			return "VBO";
		}
		case BufferTarget::IBO:
		{
			return "IBO";
		}
		case BufferTarget::SBO:
		{
			return "SBO";
		}
	}

	return "Invalid";
}

Rendy::AbstractBuffer::AbstractBuffer(BufferTarget target, uint32_t size, 
	const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("target", to_string(target).c_str());
	OPTICK_TAG("size", size);

	this->target = target;
	set_data(size, ptr);
	generate_buffer();
}

Rendy::AbstractBuffer::~AbstractBuffer()
{
	OPTICK_EVENT();
	glDeleteBuffers(1, &id);
}

Rendy::BufferTarget Rendy::AbstractBuffer::get_target() const
{
	return target;
}

uint32_t Rendy::AbstractBuffer::get_size() const
{
	OPTICK_EVENT();
	OPTICK_TAG("size", data.size());
	return static_cast<uint32_t>(data.size());
}

void Rendy::AbstractBuffer::set_data(uint32_t size, const void * ptr)
{
	OPTICK_EVENT();
	data = std::move(std::vector<uint8_t>(reinterpret_cast<const uint8_t*>(ptr),
		reinterpret_cast<const uint8_t*>(ptr) + size));
}

void Rendy::AbstractBuffer::reload()
{
	OPTICK_EVENT();

	if (!validate())
	{
		generate_buffer();
	}
}

bool Rendy::AbstractBuffer::validate() const
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

void Rendy::AbstractBuffer::bind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	glBindBuffer(get_gl_target(), id);
}

void Rendy::AbstractBuffer::unbind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	glBindBuffer(get_gl_target(), 0);
}

void Rendy::AbstractBuffer::generate_buffer()
{
	OPTICK_EVENT();
	glGenBuffers(1, &id);
	OPTICK_TAG("id", id);
	bind();
	glBufferData(get_gl_target(), data.size(), &data[0], GL_STATIC_DRAW);
}

GLenum Rendy::AbstractBuffer::get_gl_target() const
{
	OPTICK_EVENT();
	return (target == BufferTarget::VBO) ? GL_ARRAY_BUFFER :
		GL_ELEMENT_ARRAY_BUFFER;

	switch (target)
	{
		case Rendy::BufferTarget::VBO:
		{
			return GL_ARRAY_BUFFER;
		}
		case Rendy::BufferTarget::IBO:
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}
		case Rendy::BufferTarget::SBO:
		{
			return GL_SHADER_STORAGE_BUFFER;
		}
	}

	assert(0);
	return GL_INVALID_ENUM; //WTF?
}
