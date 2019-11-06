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

Rendy::ES3::Engine::Engine(VFSRef vfs)
{
	OPTICK_EVENT();

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);

	this->vfs = vfs;
	iem = read_texture_cube("assets/iem.dds");
	pmrem = read_texture_cube("assets/pmrem.dds");

	printf("PMREM max level: %d\n", pmrem->get_max_level());


	auto file = vfs->open_file("assets/lut.dds", Rendy::FileMode::Read);
	std::vector<uint8_t> content;
	content.resize(file->get_size());
	file->read(content.data(), file->get_size());
	Rendy::Image2DRef lut_image = std::make_shared<Image2D>(reinterpret_cast<const char*>(content.data()),
		content.size());
	lut = make_texture2d(lut_image);
	
	printf("LUT status: %d\n", lut->validate());

	generic_shader = make_shader(default_vertex_shader, default_fragment_shader);
	printf("GENERIC SHADER STATUS: %d\n", generic_shader->validate());
}


void Rendy::ES3::Engine::push(AbstractDrawableRef drawable, const glm::mat4& model,
	const glm::mat4& view, const glm::mat4& proj)
{
	OPTICK_EVENT();

	auto batch_list = drawable->generate_batch_list(model, view, proj);

	for (const auto& batch: batch_list)
	{
		batches.emplace_back(batch);
	}
}

void Rendy::ES3::Engine::flush()
{
	OPTICK_EVENT();

	for (auto& batch: batches)
	{
		auto command_list = batch.to_command_list();

		for (auto& command: command_list)
		{
			command->execute();
		}
	}

	batches.clear();
}

void Rendy::ES3::Engine::reload()
{
	OPTICK_EVENT();
	generic_shader->reload();
	iem->reload();
}

Rendy::AbstractShaderRef Rendy::ES3::Engine::make_shader(const std::string& vtx, const std::string& frg)
{
	OPTICK_EVENT();
	return std::make_shared<Shader>(vtx, frg);
}

Rendy::AbstractMaterialRef Rendy::ES3::Engine::make_material(ImageSetRef image_set)
{
	OPTICK_EVENT();

	AbstractMaterialRef material;

	AbstractTexture2DRef albedo;
	AbstractTexture2DRef normal;
	AbstractTexture2DRef metallic_roughness;

	if (image_set->color)
	{
		albedo = make_texture2d(image_set->color);
	}

	if (image_set->normal)
	{
		normal = make_texture2d(image_set->normal);
	}

	if (image_set->metallic_roughness)
	{
		metallic_roughness = make_texture2d(image_set->metallic_roughness);
	}

	material = std::make_shared<DefaultMaterial>(albedo, metallic_roughness, normal, iem, pmrem, lut, generic_shader);

	return material;
}

Rendy::AbstractTexture2DRef Rendy::ES3::Engine::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return std::make_shared<Texture2D>(image);
}

Rendy::AbstractTextureCubeRef Rendy::ES3::Engine::make_texture_cube(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return std::make_shared<ES3::TextureCube>(ptr, size);
}

Rendy::AbstractVertexArrayRef Rendy::ES3::Engine::make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
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

Rendy::AbstractBufferRef Rendy::ES3::Engine::make_vbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	OPTICK_TAG("size", size);

	auto vbo = std::make_shared<ES3::Buffer>(BufferTarget::VBO, size, ptr);
	return vbo;
}

Rendy::AbstractBufferRef Rendy::ES3::Engine::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES3::Buffer>(BufferTarget::IBO, size, ptr);
	return ibo;
}

Rendy::IndexType Rendy::ES3::Engine::get_index_type() const
{
	OPTICK_EVENT();
	return IndexType::UnsignedInt;
}

Rendy::AbstractTextureCubeRef Rendy::ES3::Engine::read_texture_cube(const std::string& path)
{
	auto file = vfs->open_file(path, FileMode::Read);
	assert(file);
	assert(file->validate());
	std::vector<uint8_t> data;
	data.resize(static_cast<size_t>(file->get_size()));
	file->read(&data[0], file->get_size());
	return make_texture_cube(static_cast<uint32_t>(data.size()), &data[0]);
}
