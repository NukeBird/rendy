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
#include "abstract_render_pass.h"
#include <memory>

namespace Rendy
{
	class GAPI final
	{
	public:
		GAPI(OGL version);
		AbstractShaderRef make_shader(const std::string& vtx,
			const std::string& frg);
		AbstractTexture2DRef make_texture2d(Image2DRef image);
		AbstractTextureCubeRef make_texture_cube(uint32_t size, 
			const void* ptr);
		AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
			AbstractBufferRef ibo, BufferLayoutRef layout);
		AbstractBufferRef make_vbo(uint32_t size, const void* ptr);
		AbstractBufferRef make_ibo(uint32_t size, const void* ptr);
		IndexType get_index_type() const;
	private:
		BufferFactory buffer_factory;
		ShaderFactory shader_factory;
		Texture2DFactory tex2d_factory;
		TextureCubeFactory tex_cube_factory;
		VertexArrayFactory vao_factory;
		OGL version;
	};

	using GAPIRef = std::shared_ptr<GAPI>;
};