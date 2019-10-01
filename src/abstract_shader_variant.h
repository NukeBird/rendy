#pragma once
#include "abstract_resource.h"

struct AbstractShaderVariant: public AbstractResource
{
	virtual ~AbstractShaderVariant() = default; //TODO: = 0?
	virtual void bind() = 0;
	virtual void unbind() = 0;
};

using ShaderVariantRef = std::shared_ptr<AbstractShaderVariant>;