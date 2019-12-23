#pragma once
#include "abstract_render_pass.h"

namespace Rendy
{
	class DepthPrepass: public AbstractRenderPass
	{
	public:
		DepthPrepass(ShaderSourceRef executor, bool prepass_transparency = false);
		virtual void execute(const BatchList& batches);
	private:
		bool prepass_transparency;
		ShaderSourceRef executor;
	};
};