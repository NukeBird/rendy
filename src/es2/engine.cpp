#include "engine.h"

void ES2::Engine::push(BatchList batches)
{
	CommandList commands;

	for (auto& batch: batches)
	{
		auto cl = batch.to_command_list();

		//TODO: optimize command list!

		commands.insert(commands.end(), cl.begin(), cl.end());
	}

	push(commands);
}

void ES2::Engine::push(CommandList commands)
{
	for (auto& c : commands)
	{
		c->execute(); //TODO
	}
}

void ES2::Engine::flush()
{
	//TODO
}

void ES2::Engine::reload()
{
	//TODO?
}
