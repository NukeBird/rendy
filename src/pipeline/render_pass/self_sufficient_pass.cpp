#include "self_sufficient_pass.h"
#include <optick.h>

void Rendy::SelfSufficientPass::execute(const BatchList& batches)
{
	OPTICK_EVENT();

	for (auto& batch : batches)
	{
		auto command_list = batch.to_command_list();

		for (auto& command : command_list)
		{
			command->execute();
		}
	}
}