#pragma once
#include "abstract_render_pass.h"

namespace Rendy
{
	class TransparentPass: public AbstractRenderPass
	{
	public:
		TransparentPass(ShaderSourceRef executor);
		virtual void execute(const BatchList& batches);
	private:
		ShaderSourceRef executor;
	};
};