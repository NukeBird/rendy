#include "material_factory.h"
#include "default_material_es2.h"
#include "default_shader_sources_es2.hpp"
#include "default_material_es3.h"
#include "default_shader_sources_es3.hpp"
#include <cassert>
#include <optick.h>

Rendy::MaterialFactory::MaterialFactory(OGL version, 
	Texture2DFactoryRef texture_factory, ShaderFactoryRef shader_factory,
	AbstractTextureCubeRef iem, AbstractTextureCubeRef pmrem, 
	AbstractTexture2DRef lut)
{
	OPTICK_EVENT();

	this->version = version;
	this->texture_factory = texture_factory;
	this->iem = iem;
	this->pmrem = pmrem;
	this->lut = lut;

	assert(texture_factory);
	assert(shader_factory);

	load_shader(shader_factory);
	assert(shader);
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

void Rendy::MaterialFactory::load_shader(ShaderFactoryRef shader_factory)
{
	OPTICK_EVENT();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			shader = shader_factory->make(ES2::default_vertex_shader,
				ES2::default_fragment_shader);
			return;
		}
		case Rendy::OGL::ES31:
		{
			shader = shader_factory->make(ES3::default_vertex_shader,
				ES3::default_fragment_shader);
			return;
		}
	}

	//TODO: choose best
	shader = shader_factory->make(ES3::default_vertex_shader,
		ES3::default_fragment_shader);
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

	return std::make_shared<ES2::DefaultMaterial>(color, normal, iem, 
		shader);
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
	
	return std::make_shared<ES3::DefaultMaterial>(albedo, metallic_roughness, 
		normal, iem, pmrem, lut, shader);
}