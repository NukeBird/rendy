#include "abstract_material.h"
#include "../engine.h"
#include <optick.h>

Rendy::AbstractMaterial::AbstractMaterial(ShaderFactoryRef shader_factory)
{
	OPTICK_EVENT();
	this->shader_factory = shader_factory;
}

Rendy::AbstractShaderRef Rendy::AbstractMaterial::get_shader(ShaderSourceRef extra_source) const
{
	OPTICK_EVENT();
	return shader_factory->make(get_source(extra_source));
}

Rendy::ShaderSourceRef Rendy::AbstractMaterial::get_source(ShaderSourceRef extra_source) const
{
	OPTICK_EVENT();

	if(extra_source)
	{
		return source->combine(extra_source); //TODO: cache?
	}

	return source;
}

Rendy::ShaderVariantRef Rendy::AbstractMaterial::get_shader_variant(const ShaderSettings& settings,
	ShaderSourceRef extra_source)
{
	OPTICK_EVENT();

	auto shader = get_shader(extra_source);
	return shader->compile(process_settings(settings));
}

uint32_t Rendy::AbstractMaterial::get_flags() const
{
	OPTICK_EVENT();
	return flags;
}

Rendy::ShaderSettings Rendy::AbstractMaterial::process_settings(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	ShaderSettings result = settings;
	result.flags |= get_flags();
	return std::move(result);
}