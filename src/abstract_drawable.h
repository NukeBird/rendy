#pragma once
#include "abstract_resource.h"

struct AbstractDrawable: public AbstractResource
{
	virtual ~AbstractDrawable() = default;
	virtual void draw() = 0;
};