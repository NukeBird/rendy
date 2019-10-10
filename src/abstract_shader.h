#pragma once
#include "abstract_shader_variant.h"
#include "abstract_resource.h"
#include "shader_settings.h"
#include "shader_flags.h"
#include <memory>
#include <string>

struct AbstractShader: public AbstractResource
{
	virtual ~AbstractShader() = default; //TODO: = 0?
	virtual ShaderVariantRef compile(const ShaderSettings& settings) = 0;
	virtual uint32_t get_variant_count() const = 0;
};

using AbstractShaderRef = std::shared_ptr<AbstractShader>;