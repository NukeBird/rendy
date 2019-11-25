#include "es3.h"
#include "common.h"
#include <cassert>
#include <optick.h>

Rendy::AbstractShaderRef Rendy::ES3::GAPI::make_shader(const std::string& vtx,
	const std::string& frg)
{
	OPTICK_EVENT();
	return std::make_shared<ES3::Shader>(vtx, frg);
}

Rendy::AbstractTexture2DRef Rendy::ES3::GAPI::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return std::make_shared<ES3::Texture2D>(image);
}

Rendy::AbstractTextureCubeRef Rendy::ES3::GAPI::make_texture_cube(uint32_t size,
	const void* ptr)
{
	OPTICK_EVENT();
	return std::make_shared<ES3::TextureCube>(ptr, size);
}

Rendy::AbstractVertexArrayRef Rendy::ES3::GAPI::make_vao(AbstractBufferRef vbo,
	AbstractBufferRef ibo, BufferLayoutRef layout)
{
	OPTICK_EVENT();

	assert(vbo);
	assert(vbo->get_target() == BufferTarget::VBO); //TODO

	if (ibo)
	{
		assert(ibo->get_target() == BufferTarget::IBO); //TODO
	}

	return std::make_shared<ES3::VertexArray>(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::ES3::GAPI::make_vbo(uint32_t size,
	const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	return std::make_shared<ES3::Buffer>(BufferTarget::VBO, size, ptr);
}

Rendy::AbstractBufferRef Rendy::ES3::GAPI::make_ibo(uint32_t size,
	const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	return std::make_shared<ES3::Buffer>(BufferTarget::IBO, size, ptr);
}

Rendy::IndexType Rendy::ES3::GAPI::get_index_type() const
{
	OPTICK_EVENT();
	return IndexType::UnsignedInt;
}
