#pragma once
#include "render_pass/abstract_render_pass.h"

namespace Rendy
{
	class Pipeline final: public AbstractResource
	{
	public:
		void execute(const BatchList& batches);
		virtual void reload() override;
		virtual bool validate() const override;
		void add_pass(AbstractRenderPassRef pass);
		uint32_t get_pass_count() const;
		AbstractRenderPassRef get_pass(uint32_t index) const;
	private:
		RenderPassList passes;
	};

	using PipelineRef = std::shared_ptr<Pipeline>;
};