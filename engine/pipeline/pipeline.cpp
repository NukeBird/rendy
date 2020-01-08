#include "pipeline.h"
#include <cassert>
#include <optick.h>

void Rendy::Pipeline::execute(const BatchList& batches)
{
	OPTICK_EVENT();

	for (auto& pass : passes)
	{
		pass->execute(batches);
	}
}

void Rendy::Pipeline::reload()
{
	OPTICK_EVENT();

	for (auto& pass : passes)
	{
		pass->reload();
	}
}

bool Rendy::Pipeline::validate() const
{
	OPTICK_EVENT();

	for (auto& pass : passes)
	{
		if (!pass->validate())
		{
			return false;
		}
	}

	return true;
}

void Rendy::Pipeline::add_pass(AbstractRenderPassRef pass)
{
	OPTICK_EVENT();
	passes.emplace_back(pass);
}

uint32_t Rendy::Pipeline::get_pass_count() const
{
	OPTICK_EVENT();
	return static_cast<uint32_t>(passes.size());
}

Rendy::AbstractRenderPassRef Rendy::Pipeline::get_pass(uint32_t index) const
{
	OPTICK_EVENT();

	assert(index < get_pass_count());
	return passes[index];
}