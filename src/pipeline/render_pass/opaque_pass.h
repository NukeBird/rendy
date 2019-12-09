#pragma once
#include "abstract_render_pass.h"

namespace Rendy
{
	class OpaquePass: public AbstractRenderPass
	{
	public: 
		OpaquePass(bool depth_prepassed = false);
		virtual void execute(const BatchList& batches);
	private:
		bool depth_prepassed;
	};
};