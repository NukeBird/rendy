#pragma once
#include "index_type.h"
#include "abstract_buffer.h"
#include "abstract_drawable.h"
#include "abstract_vfs.h"
#include "abstract_material.h"
#include "abstract_shader.h"
#include "abstract_texture2d.h"
#include "abstract_texture_cube.h"
#include "abstract_render_pass.h"
#include <memory>

namespace Rendy
{
	struct AbstractGAPI
	{
		virtual AbstractShaderRef make_shader(const std::string& vtx,
			const std::string& frg) = 0;
		virtual AbstractTexture2DRef make_texture2d(Image2DRef image) = 0;
		virtual AbstractTextureCubeRef make_texture_cube(uint32_t size, 
			const void* ptr) = 0;
		virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
			AbstractBufferRef ibo, BufferLayoutRef layout) = 0;
		virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) = 0;
		virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) = 0;
		virtual IndexType get_index_type() const = 0; //TODO: move to capabilities?
	};

	using AbstractGAPIRef = std::shared_ptr<AbstractGAPI>;
};