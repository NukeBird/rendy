#pragma once
#include "../abstract_shader.h"
#include <unordered_map>
#include <string>

namespace ES2
{
	class Shader final: public AbstractShader
	{
	public:
		Shader(const std::string& vertex_source, 
			const std::string& fragment_source);
		virtual void reload() override;
		virtual ShaderVariantRef compile(const ShaderSettings& settings) override;
		virtual bool validate() const override;
		virtual uint32_t get_variant_count() const override;
	private:
		std::unordered_map<ShaderSettings, ShaderVariantRef> variants;
		std::string vertex_source;
		std::string fragment_source;
	};
};