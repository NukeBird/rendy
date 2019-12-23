#pragma once
#include "../abstract_resource.h"
#include "../shader/abstract_shader.h"
#include "../command/abstract_command.h"
#include "../shader/shader_source.h"
#include "../shader/shader_factory.h"
#include <unordered_map>
#include <memory>
#include <vector>

namespace Rendy
{
	class Engine;
	using EngineRef = std::shared_ptr<Engine>;

	class AbstractMaterial: public AbstractResource
	{
	public:
		AbstractMaterial(ShaderFactoryRef shader_factory);
		virtual ~AbstractMaterial() = default;
		AbstractShaderRef get_shader(ShaderSourceRef extra_source = nullptr) const;
		ShaderSourceRef get_source(ShaderSourceRef extra_source = nullptr) const;
		ShaderVariantRef get_shader_variant(const ShaderSettings& settings, 
			ShaderSourceRef extra_source = nullptr);
		uint32_t get_flags() const;
		virtual std::vector<CommandRef> to_command_list(const ShaderSettings& settings, 
			ShaderSourceRef extra_source = nullptr) = 0;
		virtual void reload() = 0;
		virtual void reload(ShaderSourceRef extra_source) = 0;
		virtual bool validate() const = 0;
		virtual bool validate(ShaderSourceRef extra_source) const = 0;
		virtual bool uses_transparency() const = 0;
		virtual ShaderSettings process_settings(const ShaderSettings& settings);
	protected:
		ShaderSourceRef source;
		uint32_t flags = 0;
		ShaderFactoryRef shader_factory;
	};

	using AbstractMaterialRef = std::shared_ptr<AbstractMaterial>;
};