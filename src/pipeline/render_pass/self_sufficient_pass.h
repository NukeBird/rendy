#pragma once
#include "abstract_render_pass.h"

namespace Rendy
{
	class SelfSufficientPass: public AbstractRenderPass
	{
	public:
		SelfSufficientPass(ShaderSourceRef executor);
		virtual void execute(const BatchList& batches);
	private:
		ShaderSourceRef executor;
	};
};