#pragma once
#include "abstract_vertex_array.h"
#include "abstract_buffer.h"
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "abstract_shader.h"
#include "abstract_texture2d.h"
#include "abstract_texture_cube.h"
#include "abstract_material.h"
#include "image_set.h"
#include "index_type.h"
#include "batch.h"
#include <vector>
#include <string>
#include <memory>

namespace Rendy
{
	using CommandList = std::vector<CommandRef>; //TODO: move?

	struct AbstractEngine
	{
		virtual void push(AbstractDrawableRef drawable, const glm::mat4& model, 
			const glm::mat4& view, const glm::mat4& proj) = 0;
		virtual void flush() = 0;
		virtual void reload() = 0;

		virtual AbstractShaderRef make_shader(const std::string& vtx, const std::string& frg) = 0;
		virtual AbstractMaterialRef make_material(ImageSetRef image_set) = 0;
		virtual AbstractTexture2DRef make_texture2d(Image2DRef image) = 0;
		virtual AbstractTextureCubeRef make_texture_cube(uint32_t size, const void* ptr) = 0;
		virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
			BufferLayoutRef layout) = 0;
		virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) = 0;
		virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) = 0;
		virtual IndexType get_index_type() const = 0;
	};

	using AbstractEngineRef = std::shared_ptr<AbstractEngine>;
};