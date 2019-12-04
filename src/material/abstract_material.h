#pragma once
#include "../abstract_resource.h"
#include "../shader/abstract_shader.h"
#include "../command/abstract_command.h"
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractMaterial : public AbstractResource
	{
		virtual AbstractShaderRef get_shader() = 0;
		virtual ShaderVariantRef get_shader_variant(const ShaderSettings& settings);
		virtual uint32_t get_flags() const = 0;
		virtual std::vector<CommandRef> to_command_list(const ShaderSettings& settings) = 0;
		virtual void bind(const ShaderSettings& settings) = 0;
		virtual void unbind(const ShaderSettings& settingss) = 0;
		virtual ShaderSettings process_settings(const ShaderSettings& settings);
	};

	using AbstractMaterialRef = std::shared_ptr<AbstractMaterial>;
};