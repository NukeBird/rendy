#pragma once
#include "abstract_resource.h"
#include <glm/glm.hpp>

struct AbstractDrawable: public AbstractResource
{
	virtual ~AbstractDrawable() = default;
	virtual void draw(const glm::mat4& transform = glm::mat4(1.0)) = 0;
};