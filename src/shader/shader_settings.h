#pragma once
#include "shader_flags.h"
#include <string>
#include <array>
#include <map>

namespace Rendy
{
	struct ShaderSettings
	{
		uint32_t flags = 0;
		std::map<std::string, uint32_t> custom_definitions;

		std::string generate_definitions() const;

		bool operator==(const ShaderSettings& s) const;
	};
};

namespace std
{
	template <>
	struct hash<Rendy::ShaderSettings>
	{
		std::size_t operator()(const Rendy::ShaderSettings& s) const
		{
			auto uh = std::hash<uint32_t>();
			auto sh = std::hash<std::string>();
			std::size_t value = uh(s.flags);

			for (auto& i : s.custom_definitions)
			{
				value ^= sh(i.first);
				value ^= uh(i.second);
			}

			return value;
		}
	};
};