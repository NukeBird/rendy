#include "buffer_factory.h"
#include "buffer_es2.h"
#include "buffer_es3.h"

Rendy::BufferFactory::BufferFactory(OGL version)
{
	this->version = version;
}

Rendy::AbstractBufferRef Rendy::BufferFactory::make(BufferTarget target, 
	uint32_t size, const void* ptr)
{
	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::Buffer>(target, size, ptr);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::Buffer>(target, size, ptr);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::Buffer>(target, size, ptr);
}