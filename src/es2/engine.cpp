#include "es2.h"
#include "engine.h"
#include "default_material.h"
#include <optick.h>
#include <cassert>
#include <vector>

#include "default_shader_sources.hpp"

Rendy::ES2::Engine::Engine(VFSRef vfs)
{
	OPTICK_EVENT();

	this->gapi = std::make_shared<ES2::GAPI>();
	this->vfs = vfs;

	auto iem_file = vfs->open_file("assets/iem_ldr.dds", FileMode::Read);
	assert(iem_file);
	assert(iem_file->validate());
	std::vector<uint8_t> iem_data;
	iem_data.resize(static_cast<size_t>(iem_file->get_size()));
	iem_file->read(iem_data.data(), iem_file->get_size());

	iem = make_texture_cube(static_cast<uint32_t>(iem_data.size()), iem_data.data());

	Log::info("IEM max level: {0}\n", iem->get_max_level());

	generic_shader = make_shader(default_vertex_shader, default_fragment_shader);
	Log::info("GENERIC SHADER STATUS: {0}\n", generic_shader->validate());
}

void Rendy::ES2::Engine::reload()
{
	OPTICK_EVENT();
	AbstractEngine::reload();
	generic_shader->reload();
	iem->reload();
}

Rendy::AbstractGAPIRef Rendy::ES2::Engine::get_gapi() const
{
	return gapi;
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