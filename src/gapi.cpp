#include "gapi.h"
#include <optick.h>

Rendy::GAPI::GAPI(OGL version)
{
	OPTICK_EVENT();

	this->version = version;
	buffer_factory = std::make_shared<BufferFactory>(version);
	shader_factory = std::make_shared<ShaderFactory>(version);
	tex2d_factory = std::make_shared<Texture2DFactory>(version);
	tex_cube_factory = std::make_shared<TextureCubeFactory>(version);
	vao_factory = std::make_shared<VertexArrayFactory>(version);
}

Rendy::AbstractShaderRef Rendy::GAPI::make_shader(const std::string& vtx,
	const std::string& frg)
{
	OPTICK_EVENT();
	return shader_factory->make(vtx, frg);
}

Rendy::AbstractTexture2DRef Rendy::GAPI::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return tex2d_factory->make(image);
}

Rendy::AbstractTextureCubeRef Rendy::GAPI::make_texture_cube(uint32_t size,
	const void* ptr)
{
	OPTICK_EVENT();
	return tex_cube_factory->make(ptr, size);
}

Rendy::AbstractVertexArrayRef Rendy::GAPI::make_vao(AbstractBufferRef vbo,
	AbstractBufferRef ibo, BufferLayoutRef layout)
{
	OPTICK_EVENT();
	return vao_factory->make(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::GAPI::make_vbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return buffer_factory->make(BufferTarget::VBO, size, ptr);
}

Rendy::AbstractBufferRef Rendy::GAPI::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return buffer_factory->make(BufferTarget::IBO, size, ptr);
}

Rendy::BufferFactoryRef Rendy::GAPI::get_buffer_factory() const
{
	OPTICK_EVENT();
	return buffer_factory;
}

Rendy::ShaderFactoryRef Rendy::GAPI::get_shader_factory() const
{
	OPTICK_EVENT();
	return shader_factory;
}

Rendy::Texture2DFactoryRef Rendy::GAPI::get_texture2d_factory() const
{
	OPTICK_EVENT();
	return tex2d_factory;
}

Rendy::TextureCubeFactoryRef Rendy::GAPI::get_texture_cube_factory() const
{
	OPTICK_EVENT();
	return tex_cube_factory;
}

Rendy::VertexArrayFactoryRef Rendy::GAPI::get_vertex_array_factory() const
{
	OPTICK_EVENT();
	return vao_factory;
}

Rendy::IndexType Rendy::GAPI::get_index_type() const
{
	OPTICK_EVENT();

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