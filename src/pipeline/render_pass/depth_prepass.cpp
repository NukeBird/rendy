#include "../../common.h"
#include "depth_prepass.h"
#include <algorithm>

Rendy::DepthPrepass::DepthPrepass(bool prepass_transparency)
{
	this->prepass_transparency = prepass_transparency;
}

void Rendy::DepthPrepass::execute(const BatchList& batches)
{
	glDepthFunc(GL_LESS);
	glColorMask(0, 0, 0, 0);
	glDepthMask(GL_TRUE);

	for (const auto& batch: batches)
	{
		if (!batch.material->uses_transparency() || prepass_transparency)
		{
			auto command_list = batch.to_command_list();
			for (auto& command : command_list)
			{
				command->execute();
			}
		}
	}
}