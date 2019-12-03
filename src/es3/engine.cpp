#include "engine.h"
#include <optick.h>
#include <cassert>
#include <vector>

Rendy::ES3::Engine::Engine(VFSRef vfs)
{
	OPTICK_EVENT();

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);

	this->gapi = std::make_shared<GAPI>(OGL::ES31);
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

	this->material_factory = std::make_shared<MaterialFactory>(OGL::ES31,
		gapi->get_texture2d_factory(), gapi->get_shader_factory(), iem, pmrem, 
		lut);
}

void Rendy::ES3::Engine::reload()
{
	OPTICK_EVENT();
	AbstractEngine::reload();
	generic_shader->reload();
	iem->reload();
}

Rendy::GAPIRef Rendy::ES3::Engine::get_gapi() const
{
	return gapi;
}

Rendy::AbstractMaterialRef Rendy::ES3::Engine::make_material(ImageSetRef image_set)
{
	OPTICK_EVENT();
	return material_factory->make(image_set);
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
