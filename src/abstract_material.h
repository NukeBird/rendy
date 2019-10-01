#pragma once
#include "abstract_resource.h"
#include <memory>

struct AbstractMaterial: public AbstractResource
{
	virtual ~AbstractMaterial() = default; //TODO: = 0?
	virtual uint32_t get_flags() const = 0;
	virtual void bind(uint32_t extra_flags) = 0;
	virtual void unbind() = 0;
};

using AbstractMaterialRef = std::shared_ptr<AbstractMaterial>;