#include "abstract_render_pass.h"
#include <optick.h>

void Rendy::AbstractRenderPass::reload()
{
	//DO NOTHING
	OPTICK_EVENT();
}

bool Rendy::AbstractRenderPass::validate() const
{
	OPTICK_EVENT();
	return true;
}
