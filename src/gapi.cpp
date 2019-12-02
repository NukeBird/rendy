#include "gapi.h"

Rendy::GAPI::GAPI(OGL version):
	buffer_factory(version), shader_factory(version), tex2d_factory(version),
	tex_cube_factory(version), vao_factory(version)
{
	this->version = version;
}

Rendy::AbstractShaderRef Rendy::GAPI::make_shader(const std::string& vtx,
	const std::string& frg)
{
	return shader_factory.make(vtx, frg);
}

Rendy::AbstractTexture2DRef Rendy::GAPI::make_texture2d(Image2DRef image)
{
	return tex2d_factory.make(image);
}

Rendy::AbstractTextureCubeRef Rendy::GAPI::make_texture_cube(uint32_t size,
	const void* ptr)
{
	return tex_cube_factory.make(ptr, size);
}

Rendy::AbstractVertexArrayRef Rendy::GAPI::make_vao(AbstractBufferRef vbo,
	AbstractBufferRef ibo, BufferLayoutRef layout)
{
	return vao_factory.make(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::GAPI::make_vbo(uint32_t size, const void* ptr)
{
	return buffer_factory.make(BufferTarget::VBO, size, ptr);
}

Rendy::AbstractBufferRef Rendy::GAPI::make_ibo(uint32_t size, const void* ptr)
{
	return buffer_factory.make(BufferTarget::IBO, size, ptr);
}

Rendy::IndexType Rendy::GAPI::get_index_type() const
{
	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return IndexType::UnsignedShort;
		}
		case Rendy::OGL::ES31:
		{
			return IndexType::UnsignedInt;
		}
	}

	//TODO: choose best
	return IndexType::UnsignedInt;
}