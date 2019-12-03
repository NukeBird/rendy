#include "engine.h"
#include <optick.h>
#include <cassert>
#include <vector>

Rendy::ES2::Engine::Engine(VFSRef vfs) 
{
	OPTICK_EVENT();

	this->gapi = std::make_shared<GAPI>(OGL::ES20);
	this->vfs = vfs;

	auto iem_file = vfs->open_file("assets/iem_ldr.dds", FileMode::Read);
	assert(iem_file);
	assert(iem_file->validate());
	std::vector<uint8_t> iem_data;
	iem_data.resize(static_cast<size_t>(iem_file->get_size()));
	iem_file->read(iem_data.data(), iem_file->get_size());

	iem = make_texture_cube(static_cast<uint32_t>(iem_data.size()), iem_data.data());

	Log::info("IEM max level: {0}\n", iem->get_max_level());

	this->material_factory = std::make_shared<MaterialFactory>(OGL::ES20,
		gapi->get_texture2d_factory(), gapi->get_shader_factory(), iem);
}

void Rendy::ES2::Engine::reload()
{
	OPTICK_EVENT();

	AbstractEngine::reload();
	generic_shader->reload();
	iem->reload();
}

Rendy::GAPIRef Rendy::ES2::Engine::get_gapi() const
{
	OPTICK_EVENT();
	return gapi;
}

Rendy::AbstractMaterialRef Rendy::ES2::Engine::make_material(ImageSetRef image_set)
{
	OPTICK_EVENT();
	return material_factory->make(image_set);
}