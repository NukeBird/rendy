#include "engine.h"
#include "../material/es3/default_material.h"
#include "es3.h"
#include <optick.h>
#include <cassert>
#include <vector>

#include "default_shader_sources.hpp"

Rendy::ES3::Engine::Engine(VFSRef vfs)
{
	OPTICK_EVENT();

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);

	this->gapi = std::make_shared<ES3::GAPI>();
	this->vfs = vfs;
	iem = read_texture_cube("assets/iem.dds");
	pmrem = read_texture_cube("assets/pmrem.dds");

	Log::info("PMREM max level: {0}", pmrem->get_max_level());


	auto file = vfs->open_file("assets/lut.dds", Rendy::FileMode::Read);
	std::vector<uint8_t> content;
	content.resize(file->get_size());
	file->read(content.data(), file->get_size());
	Rendy::Image2DRef lut_image = std::make_shared<Image2D>(reinterpret_cast<const char*>(content.data()),
		content.size());
	lut = make_texture2d(lut_image);
	lut->set_wrap_mode(WrapMode::ClampToEdge);
	lut->set_min_filter(MinFilter::Linear);
	lut->set_mag_filter(MagFilter::Linear);
	
	Log::info("LUT status: {0}", lut->validate());

	generic_shader = make_shader(default_vertex_shader, default_fragment_shader);
	Log::info("GENERIC SHADER STATUS: {0}", generic_shader->validate());
}

void Rendy::ES3::Engine::reload()
{
	OPTICK_EVENT();
	AbstractEngine::reload();
	generic_shader->reload();
	iem->reload();
}

Rendy::AbstractGAPIRef Rendy::ES3::Engine::get_gapi() const
{
	return gapi;
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
