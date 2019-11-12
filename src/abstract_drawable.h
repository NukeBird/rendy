#pragma once
#include "abstract_resource.h"
#include "batch.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Rendy
{
	using BatchList = std::vector<Batch>;

	struct AbstractDrawable : public AbstractResource
	{
		virtual BatchList generate_batch_list(const glm::mat4& model,
			const glm::mat4& view, const glm::mat4& proj) = 0;
		virtual void update(float dt) = 0;
	};

	using AbstractDrawableRef = std::shared_ptr<AbstractDrawable>;
};