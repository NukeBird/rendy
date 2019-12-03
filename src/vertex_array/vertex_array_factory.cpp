#include "vertex_array_factory.h"
#include "vertex_array_es2.h"
#include "vertex_array_es3.h"
#include <optick.h>

Rendy::VertexArrayFactory::VertexArrayFactory(OGL version)
{
	OPTICK_EVENT();
	this->version = version;
}

Rendy::AbstractVertexArrayRef Rendy::VertexArrayFactory::make(AbstractBufferRef vbo,
	AbstractBufferRef ibo, const BufferLayoutRef& layout)
{
	OPTICK_EVENT();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::VertexArray>(vbo, ibo, layout);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::VertexArray>(vbo, ibo, layout);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::VertexArray>(vbo, ibo, layout);
}