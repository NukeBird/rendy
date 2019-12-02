#include "vertex_array_factory.h"
#include "vertex_array_es2.h"
#include "vertex_array_es3.h"

Rendy::VertexArrayFactory::VertexArrayFactory(OGL version)
{
	this->version = version;
}

Rendy::AbstractVertexArrayRef Rendy::VertexArrayFactory::make(AbstractBufferRef vbo,
	AbstractBufferRef ibo, const BufferLayoutRef& layout)
{
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