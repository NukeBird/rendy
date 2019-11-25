#include "abstract_engine.h"

bool Rendy::AbstractEngine::validate()
{
	for (auto& stage: render_stages)
	{
		if (!stage->validate())
		{
			return false;
		}
	}

	return true;
}

uint32_t Rendy::AbstractEngine::get_stage_count() const
{
	return static_cast<uint32_t>(render_stages.size());
}

Rendy::AbstractRenderStageRef Rendy::AbstractEngine::get_stage(uint32_t index)
{
	assert(static_cast<size_t>(index) < render_stages.size());
	return render_stages[index];
}

void Rendy::AbstractEngine::add_stage(AbstractRenderStageRef stage)
{
	render_stages.emplace_back(stage);
}
