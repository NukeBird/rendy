#include "abstract_material.h"
#include <optick.h>

Rendy::ShaderVariantRef Rendy::AbstractMaterial::get_shader_variant(uint32_t extra_flags)
{
	OPTICK_EVENT();

	ShaderSettings settings;
	settings.flags = extra_flags | get_flags();
	return get_shader()->compile(settings);
}