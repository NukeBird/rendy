#include "material_factory.h"
#include "default_material_es2.h"
#include "default_shader_sources_es2.hpp"
#include "default_material_es3.h"
#include "default_shader_sources_es3.hpp"
#include "../engine.h"
#include <cassert>
#include <optick.h>

Rendy::MaterialFactory::MaterialFactory(OGL version, Texture2DFactoryRef texture_factory, 
	ShaderFactoryRef shader_factory, AbstractTextureCubeRef iem, AbstractTextureCubeRef pmrem, 
	AbstractTexture2DRef lut)
{
	OPTICK_EVENT();

	this->version = version;
	this->texture_factory = texture_factory;
	this->shader_factory = shader_factory;
	this->iem = iem;
	this->pmrem = pmrem;
	this->lut = lut;

	assert(texture_factory);
	assert(shader_factory);

	load_shader_source();
	assert(shader_source);
	assert(iem);

	if (version == OGL::ES31 || version == OGL::ChooseBest) //TODO: choose best
	{
		assert(lut);
		assert(pmrem);
	}
}

Rendy::AbstractMaterialRef Rendy::MaterialFactory::make(ImageSetRef image_set)
{
	OPTICK_EVENT();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return make_es2(image_set);
		}
		case Rendy::OGL::ES31:
		{
			return make_es3(image_set);
		}
	}

	//TODO: choose best
	return make_es3(image_set);
}

void Rendy::MaterialFactory::load_shader_source()
{
	OPTICK_EVENT();

	shader_source = std::make_shared<ShaderSource>();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			shader_source->set_source(ShaderType::VertexShader, ES2::default_vertex_shader);
			shader_source->set_source(ShaderType::FragmentShader, ES2::default_fragment_shader);
			return;
		}
		case Rendy::OGL::ES31:
		{
			shader_source->set_source(ShaderType::VertexShader, ES3::default_vertex_shader);
			shader_source->set_source(ShaderType::FragmentShader, ES3::default_fragment_shader);
			return;
		}
	}

	shader_source->set_source(ShaderType::VertexShader, ES3::default_vertex_shader);
	shader_source->set_source(ShaderType::FragmentShader, ES3::default_fragment_shader);
}

Rendy::AbstractMaterialRef Rendy::MaterialFactory::make_es2(ImageSetRef image_set)
{
	OPTICK_EVENT();

	AbstractTexture2DRef color;
	AbstractTexture2DRef normal;

	if (image_set->color)
	{
		color = texture_factory->make(image_set->color);
	}

	if (image_set->normal)
	{
		normal = texture_factory->make(image_set->normal);
	}

	return std::make_shared<ES2::DefaultMaterial>(shader_factory, color, normal, iem, 
		shader_source);
}

Rendy::AbstractMaterialRef Rendy::MaterialFactory::make_es3(ImageSetRef image_set)
{
	OPTICK_EVENT();

	AbstractTexture2DRef albedo;
	AbstractTexture2DRef normal;
	AbstractTexture2DRef metallic_roughness;

	if (image_set->color)
	{
		albedo = texture_factory->make(image_set->color);
	}

	if (image_set->normal)
	{
		normal = texture_factory->make(image_set->normal);
	}

	if (image_set->metallic_roughness)
	{
		metallic_roughness = texture_factory->make(image_set->metallic_roughness);
	}
	
	return std::make_shared<ES3::DefaultMaterial>(shader_factory, albedo, metallic_roughness,
		normal, iem, pmrem, lut, shader_source);
}