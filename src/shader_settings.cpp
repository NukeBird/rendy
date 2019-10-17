#include "shader_settings.h"
#include <glm/glm.hpp>

std::string ShaderSettings::generate_definitions() const
{
	std::string definitions = shader_flags_to_defines(flags);
	
	if (flags & USE_DIRECT_LIGHTS)
	{
		definitions += "#define DIRECT_LIGHT_COUNT " + 
			std::to_string(glm::max(properties[DIRECT_LIGHT_COUNT], 1U))
			+ "\n";
	}

	return definitions;
}

bool ShaderSettings::operator==(const ShaderSettings& s) const
{
	if (flags == s.flags)
	{
		if (flags & USE_DIRECT_LIGHTS)
		{
			if (properties[DIRECT_LIGHT_COUNT] == s.properties[DIRECT_LIGHT_COUNT])
			{
				return true;
			}

			return false;
		}

		return true;
	}

	return false;
}
