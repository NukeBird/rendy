#pragma once
#include "ogl.hpp"
#include "index_type.h"
#include "buffer/buffer_factory.h"
#include "drawable/abstract_drawable.h"
#include "util/vfs/abstract_vfs.h"
#include "material/abstract_material.h"
#include "shader/shader_factory.h"
#include "texture/texture2d/texture2d_factory.h"
#include "texture/texture_cube/texture_cube_factory.h"
#include "vertex_array/vertex_array_factory.h"
#include <memory>

namespace Rendy
{
	class GAPI final
	{
	public:
		GAPI(OGL version);
		AbstractShaderRef make_shader(const std::string& vtx,
			const std::string& frg);
		AbstractShaderRef make_shader(ShaderSourceRef source);
		AbstractTexture2DRef make_texture2d(Image2DRef image);
		AbstractTextureCubeRef make_texture_cube(uint32_t size, 
			const void* ptr);
		AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
			AbstractBufferRef ibo, BufferLayoutRef layout);
		AbstractBufferRef make_vbo(uint32_t size, const void* ptr);
		AbstractBufferRef make_ibo(uint32_t size, const void* ptr);
		AbstractBufferRef make_sbo(uint32_t size, const void* ptr);
		AbstractBufferRef make_cbo(uint32_t size, const void* ptr);
		BufferFactoryRef get_buffer_factory() const;
		ShaderFactoryRef get_shader_factory() const;
		Texture2DFactoryRef get_texture2d_factory() const;
		TextureCubeFactoryRef get_texture_cube_factory() const;
		VertexArrayFactoryRef get_vertex_array_factory() const;
		IndexType get_index_type() const;
	private:
		BufferFactoryRef buffer_factory;
		ShaderFactoryRef shader_factory;
		Texture2DFactoryRef tex2d_factory;
		TextureCubeFactoryRef tex_cube_factory;
		VertexArrayFactoryRef vao_factory;
		OGL version;
	};

	using GAPIRef = std::shared_ptr<GAPI>;
};