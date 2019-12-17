#pragma once
#include "abstract_shader.h"
#include "shader_source.h"
#include <unordered_map>
#include <string>

namespace Rendy
{
	namespace ES2
	{
		class Shader final: public AbstractShader
		{
		public:
			Shader(ShaderSourceRef source);
			Shader(const std::string& vertex_source,
				const std::string& fragment_source);
			virtual std::string generate_meta(const ShaderSettings& settings) override;
			virtual ShaderVariantRef make_variant(const std::string& vertex_source,
				const std::string& fragment_source) override;
		};
	};
};