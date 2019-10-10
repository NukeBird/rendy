#pragma once
#include "shader_flags.h"
#include <unordered_map>
#include <string>
#include <array>

enum ShaderProperty
{
	DIRECT_LIGHT_COUNT = 0,
	LAST = 1
};

struct ShaderSettings
{
	uint32_t flags;
	std::array<uint32_t, ShaderProperty::LAST> properties;

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

			for (size_t i = 0; i < ShaderProperty::LAST; ++i)
			{
				value ^= h(s.properties[i]);
			}

			return value;
		}
	};
};