#pragma once
#include "../ogl.hpp"
#include "abstract_vertex_array.h"
#include <memory>
#include <string>

namespace Rendy
{
	class VertexArrayFactory final
	{
	public:
		VertexArrayFactory(OGL version);
		AbstractVertexArrayRef make(AbstractBufferRef vbo, AbstractBufferRef ibo,
			const BufferLayoutRef& layout);
	private:
		OGL version;
	};

	using VertexArrayFactoryRef = std::shared_ptr<VertexArrayFactory>;
};