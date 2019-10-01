#pragma once
#include "abstract_resource.h"

struct AbstractMaterial: public AbstractResource
{
	virtual ~AbstractMaterial() = default; //TODO: = 0?
	virtual uint32_t get_flags() const = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};