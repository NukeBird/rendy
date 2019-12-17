#pragma once
#include "abstract_shader_variant.h"
#include "../abstract_resource.h"
#include "shader_settings.h"
#include "shader_flags.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace Rendy
{
	class AbstractShader : public AbstractResource
	{
	public:
		AbstractShader(const std::string& vertex_source,
			const std::string& fragment_source);
		virtual ~AbstractShader() = default;
		virtual void reload() override;
		virtual bool validate() const override;
		virtual ShaderVariantRef compile(const ShaderSettings& settings);
		uint32_t get_variant_count() const;
	protected:
		virtual std::string generate_meta(const ShaderSettings& settings) = 0;
		virtual ShaderVariantRef make_variant(const std::string& vertex_source,
			const std::string& fragment_source) = 0;

		std::unordered_map<ShaderSettings, ShaderVariantRef> variants;
		std::string vertex_source;
		std::string fragment_source;
	};

	using AbstractShaderRef = std::shared_ptr<AbstractShader>;
};