#include "../../common.h"
#include "depth_prepass.h"
#include <algorithm>

Rendy::DepthPrepass::DepthPrepass(bool prepass_transparency)
{
	this->prepass_transparency = prepass_transparency;
}

void Rendy::DepthPrepass::execute(const BatchList& batches)
{
	BatchList filtered_batches;

	std::copy_if(batches.begin(), batches.end(), 
		std::back_inserter(filtered_batches),
		[=](const Batch& b)
		{
			return (prepass_transparency ? true : 
				b.material->uses_transparency());
		});

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDepthFunc(GL_LESS);
	glColorMask(0, 0, 0, 0);
	glDepthMask(GL_TRUE);

	for (const auto& batch : filtered_batches)
	{
		auto command_list = batch.to_command_list();
		for (auto& command : command_list)
		{
			command->execute();
		}
	}
}