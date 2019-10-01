#pragma once
#include "abstract_resource.h"
#include "abstract_shader.h"
#include <memory>

struct AbstractMaterial: public AbstractResource
{
	virtual ~AbstractMaterial() = default; //TODO: = 0?
	virtual AbstractShaderRef get_shader() = 0;
	virtual uint32_t get_flags() const = 0;
	virtual void bind(uint32_t extra_flags) = 0;
	virtual void unbind(uint32_t extra_flags) = 0;
};

using AbstractMaterialRef = std::shared_ptr<AbstractMaterial>;