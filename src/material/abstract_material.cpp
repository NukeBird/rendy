#include "abstract_material.h"
#include <optick.h>

Rendy::ShaderVariantRef Rendy::AbstractMaterial::get_shader_variant(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	return get_shader()->compile(process_settings(settings));
}

Rendy::ShaderSettings Rendy::AbstractMaterial::process_settings(const ShaderSettings& settings)
{
	ShaderSettings result = settings;
	result.flags |= get_flags();
	return std::move(result);
}