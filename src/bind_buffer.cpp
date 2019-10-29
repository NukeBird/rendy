#include "bind_buffer.h"

BindBuffer::BindBuffer(AbstractBufferRef buffer)
{
	this->buffer = buffer;
}

void BindBuffer::execute()
{
	if (buffer)
	{
		buffer->bind();
	}
}
