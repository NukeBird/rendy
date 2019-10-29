#include "engine.h"

void ES2::Engine::draw(DrawCallList draw_calls)
{
	CommandList commands;

	for (auto& dc : draw_calls)
	{
		auto cl = dc.to_command_list();

		//TODO: optimize command list!

		commands.insert(commands.end(), cl.begin(), cl.end());
	}

	draw(commands);
}

void ES2::Engine::draw(CommandList commands)
{
	for (auto& c : commands)
	{
		c->execute();
	}
}

void ES2::Engine::reload()
{
	//TODO?
}
