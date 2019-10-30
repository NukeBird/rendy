#pragma once
#include "abstract_vertex_array.h"
#include "abstract_buffer.h"
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "abstract_shader.h"
#include "index_type.h"
#include "batch.h"
#include <vector>
#include <string>

namespace Rendy
{
	using CommandList = std::vector<CommandRef>;

	struct AbstractEngine
	{
		virtual void push(BatchList batches) = 0;
		virtual void push(CommandList commands) = 0;
		virtual void flush() = 0;
		virtual void reload() = 0;

		virtual AbstractShaderRef make_shader(const std::string& vtx, const std::string& frg) = 0;
		virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
			BufferLayoutRef layout) = 0;
		virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) = 0;
		virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) = 0;
		virtual IndexType get_index_type() const = 0;
	};
};