#include "engine.h"
#include "buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture2d.h"
#include "texture_cube.h"
#include "default_material.h"
#include <optick.h>
#include <cassert>
#include <vector>

#include "default_shader_sources.hpp"

Rendy::ES2::Engine::Engine(VFSRef vfs)
{
	this->vfs = vfs;

	auto iem_file = vfs->open_file("assets/iem_ldr.dds", FileMode::Read);
	assert(iem_file);
	assert(iem_file->validate());
	std::vector<uint8_t> iem_data;
	iem_data.resize(static_cast<size_t>(iem_file->get_size()));
	iem_file->read(iem_data.data(), iem_file->get_size());

	iem = make_texture_cube(static_cast<uint32_t>(iem_data.size()), iem_data.data());

	generic_shader = make_shader(default_vertex_shader, default_fragment_shader);
	printf("GENERIC SHADER STATUS: %d\n", generic_shader->validate());
}

void Rendy::ES2::Engine::push(BatchList batches)
{
	OPTICK_EVENT();

	CommandList commands;

	for (auto& batch: batches)
	{
		auto cl = batch.to_command_list();

		//TODO: optimize command list!

		commands.insert(commands.end(), cl.begin(), cl.end());
	}

	push(commands);
}

void Rendy::ES2::Engine::push(CommandList commands)
{
	OPTICK_EVENT();

	for (auto& c : commands)
	{
		c->execute(); //TODO
	}
}

void Rendy::ES2::Engine::flush()
{
	OPTICK_EVENT();
	//TODO
}

void Rendy::ES2::Engine::reload()
{
	OPTICK_EVENT();
	//TODO?
}

Rendy::AbstractShaderRef Rendy::ES2::Engine::make_shader(const std::string& vtx, const std::string& frg)
{
	OPTICK_EVENT();
	return std::make_shared<Shader>(vtx, frg);
}

Rendy::AbstractMaterialRef Rendy::ES2::Engine::make_material(ImageSetRef image_set)
{
	OPTICK_EVENT();

	AbstractMaterialRef material;

	AbstractTexture2DRef color;
	AbstractTexture2DRef normal;

	if (image_set->color)
	{
		color = make_texture2d(image_set->color);
	}

	if (image_set->normal)
	{
		normal = make_texture2d(image_set->normal);
	}

	material = std::make_shared<DefaultMaterial>(color, normal, iem, generic_shader);

	return material;
}

Rendy::AbstractTexture2DRef Rendy::ES2::Engine::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return std::make_shared<Texture2D>(image);
}

Rendy::AbstractTextureCubeRef Rendy::ES2::Engine::make_texture_cube(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return std::make_shared<TextureCube>(ptr, size);
}

Rendy::AbstractVertexArrayRef Rendy::ES2::Engine::make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
	BufferLayoutRef layout)
{
	OPTICK_EVENT();

	assert(vbo);
	assert(vbo->get_target() == BufferTarget::VBO); //TODO

	if (ibo)
	{
		assert(ibo->get_target() == BufferTarget::IBO); //TODO
	}

	return std::make_shared<VertexArray>(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::ES2::Engine::make_vbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	OPTICK_TAG("size", size);

	auto vbo = std::make_shared<ES2::Buffer>(BufferTarget::VBO, size, ptr);
	return vbo;
}

Rendy::AbstractBufferRef Rendy::ES2::Engine::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES2::Buffer>(BufferTarget::IBO, size, ptr);
	return ibo;
}

Rendy::IndexType Rendy::ES2::Engine::get_index_type() const
{
	OPTICK_EVENT();
	return IndexType::UnsignedShort;
}
