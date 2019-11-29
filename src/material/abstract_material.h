#pragma once
#include "../abstract_resource.h"
#include "../shader/abstract_shader.h"
#include "../abstract_command.h"
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractMaterial : public AbstractResource
	{
		virtual AbstractShaderRef get_shader() = 0;
		virtual ShaderVariantRef get_shader_variant(uint32_t extra_flags) = 0;
		virtual uint32_t get_flags() const = 0;
		virtual std::vector<CommandRef> to_command_list(uint32_t extra_flags) = 0;
		virtual void bind(const ShaderSettings& settings) = 0;
		virtual void unbind(uint32_t extra_flags) = 0;
	};

	using AbstractMaterialRef = std::shared_ptr<AbstractMaterial>;
};