#include "engine.h"

void ES2::Engine::push(BatchList draw_calls)
{
	CommandList commands;

	for (auto& dc : draw_calls)
	{
		auto cl = dc.to_command_list();

		//TODO: optimize command list!

		commands.insert(commands.end(), cl.begin(), cl.end());
	}

	push(commands);
}

void ES2::Engine::push(CommandList commands)
{
	for (auto& c : commands)
	{
		c->execute();
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
