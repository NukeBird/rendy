#pragma once
#include "abstract_shader_variant.h"
#include <unordered_map>
#include <string>

namespace Rendy
{
	namespace ES3
	{
		class ShaderVariant : public AbstractShaderVariant
		{
		public:
			ShaderVariant(const std::string& vtx, const std::string& frg);
			virtual void cache_attribute_locations() override;
			virtual void cache_uniform_locations() override;
		};
	};
};