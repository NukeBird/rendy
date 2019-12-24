#include "engine.h"
#include <optick.h>
#include "rendy.h"

Rendy::Engine::Engine(OGL version, VFSRef vfs)
{
	OPTICK_EVENT();

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);

	this->gapi = std::make_shared<GAPI>(version); //TODO: process version
	this->vfs = vfs;

	iem = make_texture_cube("assets/iem.dds");

	if (version == OGL::ES31 || version == OGL::ChooseBest) //TODO
	{
		pmrem = make_texture_cube("assets/pmrem.dds");
		Log::info("PMREM max level: {0}", pmrem->get_max_level());

		auto file = vfs->open_file("assets/lut.dds", Rendy::FileMode::Read);
		std::vector<uint8_t> content;
		content.resize(file->get_size());
		file->read(content.data(), file->get_size());
		Rendy::Image2DRef lut_image = std::make_shared<Image2D>(reinterpret_cast<const char*>(content.data()),
			static_cast<uint32_t>(content.size()));
		lut = make_texture2d(lut_image);
		lut->set_wrap_mode(WrapMode::ClampToEdge);
		lut->set_min_filter(MinFilter::Linear);
		lut->set_mag_filter(MagFilter::Linear);

		Log::info("LUT status: {0}", lut->validate());
	}

	this->material_factory = std::make_shared<MaterialFactory>(version, gapi->get_texture2d_factory(),
		gapi->get_shader_factory(), iem, pmrem, lut);
}

Rendy::AbstractTextureCubeRef Rendy::Engine::make_texture_cube(const std::string& path)
{
	OPTICK_EVENT();

	auto file = vfs->open_file(path, FileMode::Read);
	assert(file);
	assert(file->validate());
	std::vector<uint8_t> data;
	data.resize(static_cast<size_t>(file->get_size()));
	file->read(&data[0], file->get_size());
	return make_texture_cube(static_cast<uint32_t>(data.size()), &data[0]);
}

void Rendy::Engine::push(AbstractDrawableRef drawable, 
	const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
{
	OPTICK_EVENT();

	auto batch_list = drawable->generate_batch_list(model, view, proj);

	for (const auto& batch : batch_list)
	{
		batches.emplace_back(batch);
	}
}

Rendy::GAPIRef Rendy::Engine::get_gapi() const
{
	OPTICK_EVENT();
	return gapi;
}

Rendy::AbstractMaterialRef Rendy::Engine::make_material(ImageSetRef image_set)
{
	OPTICK_EVENT();
	return material_factory->make(image_set);
}

void Rendy::Engine::flush()
{
	OPTICK_EVENT();

	if (pipeline)
	{
		pipeline->execute(batches);
	}

	batches.clear();
}

void Rendy::Engine::reload()
{
	OPTICK_EVENT();

	if (pipeline)
	{
		pipeline->reload();
	}

	batches.clear();
}

bool Rendy::Engine::validate()
{
	OPTICK_EVENT();

	if (pipeline)
	{
		return pipeline->validate();
	}

	return true;
}

Rendy::PipelineRef Rendy::Engine::get_pipeline() const
{
	OPTICK_EVENT();
	return pipeline;
}

void Rendy::Engine::set_pipeline(PipelineRef pipeline)
{
	OPTICK_EVENT();
	this->pipeline = pipeline;
}

Rendy::AbstractShaderRef Rendy::Engine::make_shader(const std::string& vtx, 
	const std::string& frg)
{
	OPTICK_EVENT();
	return get_gapi()->make_shader(vtx, frg);
}

Rendy::AbstractShaderRef Rendy::Engine::make_shader(ShaderSourceRef source)
{
	OPTICK_EVENT();
	return gapi->make_shader(source);
}

Rendy::AbstractTexture2DRef Rendy::Engine::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return get_gapi()->make_texture2d(image);
}

Rendy::AbstractTextureCubeRef Rendy::Engine::make_texture_cube(uint32_t size, 
	const void* ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_texture_cube(size, ptr);
}

Rendy::AbstractVertexArrayRef Rendy::Engine::make_vao(AbstractBufferRef vbo, 
	AbstractBufferRef ibo, BufferLayoutRef layout)
{
	OPTICK_EVENT();
	return get_gapi()->make_vao(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::Engine::make_vbo(uint32_t size, 
	const void* ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_vbo(size, ptr);
}

Rendy::AbstractBufferRef Rendy::Engine::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_ibo(size, ptr);
}

Rendy::AbstractBufferRef Rendy::Engine::make_sbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return gapi->make_sbo(size, ptr);
}

Rendy::AbstractBufferRef Rendy::Engine::make_cbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return gapi->make_cbo(size, ptr);
}

Rendy::IndexType Rendy::Engine::get_index_type() const
{
	OPTICK_EVENT();
	return get_gapi()->get_index_type();
}
