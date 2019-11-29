#pragma once
#include "../abstract_resource.h"
#include "../buffer/abstract_buffer.h"
#include "../shader/abstract_shader_variant.h"
#include "../buffer/buffer_layout.h"

namespace Rendy
{
	struct AbstractVertexArray: public AbstractResource
	{
		virtual void bind(const ShaderVariantRef& shader) = 0;
		virtual void draw() = 0;
		virtual void unbind() = 0;
		virtual BufferLayoutRef get_layout() const = 0;
		virtual AbstractBufferRef get_vertex_buffer() = 0;
		virtual AbstractBufferRef get_index_buffer() = 0;
	};

	using AbstractVertexArrayRef = std::shared_ptr<AbstractVertexArray>;
};