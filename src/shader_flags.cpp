#include "shader_flags.h"

inline ShaderFlag operator|(ShaderFlag a, ShaderFlag b)
{
	return static_cast<ShaderFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline bool operator&(ShaderFlag a, ShaderFlag b)
{
	return static_cast<bool>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

ShaderFlag get_all_shader_flags()
{
	ShaderFlag flags = ShaderFlag::NOTHING;

	for (const auto& f : ShaderFlag_list)
	{
		flags = flags | f;
	}

	return flags;
}

std::string shader_flags_to_defines(ShaderFlag flags)
{
	std::string result;
	
	for (const auto& f : ShaderFlag_list)
	{
		if (flags & f)
		{
			result += "#define " + ShaderFlag_to_string(f) + "\n";
		}
	}

	return result;
}
