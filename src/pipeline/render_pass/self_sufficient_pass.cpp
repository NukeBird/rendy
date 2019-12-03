#include "self_sufficient_pass.h"

void Rendy::SelfSufficientPass::execute(const BatchList& batches)
{
	for (auto& batch : batches)
	{
		auto command_list = batch.to_command_list();

		for (auto& command : command_list)
		{
			command->execute();
		}
	}
}