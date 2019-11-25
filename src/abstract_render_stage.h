#pragma once
#include "abstract_resource.h"
#include "batch.h"
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractRenderStage: public AbstractResource
	{
		virtual void execute(const BatchList& batches) = 0;
		virtual void reload() override;
		virtual bool validate() const override;
	};

	using AbstractRenderStageRef = std::shared_ptr<AbstractRenderStage>;
	using RenderStageList = std::vector<AbstractRenderStageRef>;
};