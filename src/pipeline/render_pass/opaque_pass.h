#pragma once
#include "abstract_render_pass.h"

namespace Rendy
{
	class OpaquePass: public AbstractRenderPass
	{
	public:
		virtual void execute(const BatchList& batches);
	};
};