#include "shader_settings.h"
#include <glm/glm.hpp>
#include <optick.h>

std::string ShaderSettings::generate_definitions() const
{
	OPTICK_EVENT();

	std::string definitions = shader_flags_to_defines(flags);

	return definitions;
}

bool ShaderSettings::operator==(const ShaderSettings& s) const
{
	if (flags == s.flags)
	{
		return true;
	}

	return false;
}
