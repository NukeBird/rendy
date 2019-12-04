#include "shader_settings.h"
#include <glm/glm.hpp>
#include <optick.h>

std::string Rendy::ShaderSettings::generate_definitions() const
{
	OPTICK_EVENT();

	std::string definitions = shader_flags_to_defines(flags);

	for (auto& i : custom_definitions)
	{
		definitions += "#define " + i.first + " " +
			std::to_string(i.second) + "\n";
	}

	return std::move(definitions);
}

bool compare_maps(const std::map<std::string, uint32_t>& l, 
	const std::map<std::string, uint32_t>& r)
{
	if (l.size() != r.size())
	{
		return false;  //differing sizes, they are not the same
	}

	for (auto i = l.begin(), j = r.begin(); i != l.end(); ++i, ++j)
	{
		if (*i != *j)
		{
			return false;
		}
	}

	return true;
}

bool Rendy::ShaderSettings::operator==(const ShaderSettings& s) const
{
	OPTICK_EVENT();

	if (flags != s.flags)
	{
		return false;
	}

	return compare_maps(custom_definitions, s.custom_definitions);
}
