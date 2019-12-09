#include "../../common.h"
#include "transparent_pass.h"

void Rendy::TransparentPass::execute(const BatchList& batches)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(GL_LESS);
	glColorMask(1, 1, 1, 1);
	glDepthMask(GL_TRUE);

	for (auto& batch : batches)
	{
		if (batch.material->uses_transparency())
		{
			auto command_list = batch.to_command_list();
			for (auto& command : command_list)
			{
				command->execute();
			}
		}
	}
}
