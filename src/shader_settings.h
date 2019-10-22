#pragma once
#include "shader_flags.h"
#include <unordered_map>
#include <string>
#include <array>

struct ShaderSettings
{
	uint32_t flags;

	std::string generate_definitions() const;

	bool operator==(const ShaderSettings& s) const;
};

namespace std 
{
	template <>
	struct hash<ShaderSettings>
	{
		std::size_t operator()(const ShaderSettings& s) const
		{
			auto h = std::hash<uint32_t>();
			std::size_t value = h(s.flags);

			return value;
		}
	};
};