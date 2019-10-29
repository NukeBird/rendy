#pragma once
#include "abstract_resource.h"
#include "draw_call.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

using DrawCallList = std::vector<DrawCall>;

struct AbstractDrawable: public AbstractResource
{
	virtual ~AbstractDrawable() = default;
	virtual DrawCallList generate_draw_calls(const glm::mat4& model, 
		const glm::mat4& view, const glm::mat4& proj) = 0;
};

using AbstractDrawableRef = std::shared_ptr<AbstractDrawable>;