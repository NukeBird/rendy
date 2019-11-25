#pragma once
#include "../abstract_gapi.h"

namespace Rendy
{
	namespace ES2
	{
		class GAPI: public AbstractGAPI
		{
			virtual AbstractShaderRef make_shader(const std::string& vtx, 
				const std::string& frg) override;
			virtual AbstractTexture2DRef make_texture2d(Image2DRef image) override;
			virtual AbstractTextureCubeRef make_texture_cube(uint32_t size, 
				const void* ptr) override;
			virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
				AbstractBufferRef ibo, BufferLayoutRef layout) override;
			virtual AbstractBufferRef make_vbo(uint32_t size, 
				const void* ptr) override;
			virtual AbstractBufferRef make_ibo(uint32_t size, 
				const void * ptr) override;
			virtual IndexType get_index_type() const override;
		};
	};
};