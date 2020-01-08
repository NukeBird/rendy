#include "../../common.h"
#include "transparent_pass.h"

Rendy::TransparentPass::TransparentPass(ShaderSourceRef executor)
{
	this->executor = executor;
}

void Rendy::TransparentPass::execute(const BatchList& batches)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(GL_LESS);
	glColorMask(1, 1, 1, 1);
	glDepthMask(GL_TRUE);

	for (auto& batch : batches)
	{
		if (batch.material->uses_transparency())
		{
			auto command_list = batch.to_command_list(executor);
			for (auto& command : command_list)
			{
				command->execute();
			}
		}
	}
}
