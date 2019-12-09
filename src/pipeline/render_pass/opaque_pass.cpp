#include "../../common.h"
#include "opaque_pass.h"
#include <algorithm>
#include <optick.h>

Rendy::OpaquePass::OpaquePass(bool depth_prepassed)
{
	OPTICK_EVENT();
	this->depth_prepassed = depth_prepassed;
}

void Rendy::OpaquePass::execute(const BatchList& batches)
{
	OPTICK_EVENT();

	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(depth_prepassed ? GL_EQUAL : GL_LESS);
	glColorMask(1, 1, 1, 1);
	glDepthMask(GL_TRUE);

	for (const auto& batch : batches)
	{
		if (!batch.material->uses_transparency())
		{
			auto command_list = batch.to_command_list();
			for (auto& command : command_list)
			{
				command->execute();
			}
		}
	}
}