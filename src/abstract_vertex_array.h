#pragma once
#include "abstract_resource.h"
#include "abstract_buffer.h"
#include "abstract_shader_variant.h"
#include "buffer_layout.h"

struct AbstractVertexArray: public AbstractResource
{
	virtual ~AbstractVertexArray() = default;
	virtual void bind(const ShaderVariantRef& shader) = 0;
	virtual void draw() = 0;
	virtual void unbind() = 0;
	virtual BufferLayoutRef get_layout() const = 0;
	virtual AbstractBufferRef get_vertex_buffer() = 0;
	virtual AbstractBufferRef get_index_buffer() = 0;
};

using AbstractVertexArrayRef = std::shared_ptr<AbstractVertexArray>;