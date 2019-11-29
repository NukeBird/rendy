#pragma once
#include "abstract_resource.h"
#include "batch.h"
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractRenderPass: public AbstractResource
	{
		virtual void execute(const BatchList& batches) = 0;
		virtual void reload() override;
		virtual bool validate() const override;
	};

	using AbstractRenderPassRef = std::shared_ptr<AbstractRenderPass>;
	using RenderPassList = std::vector<AbstractRenderPassRef>;
};