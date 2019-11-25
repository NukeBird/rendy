#include "abstract_engine.h"
#include <optick.h>

void Rendy::AbstractEngine::push(AbstractDrawableRef drawable, 
	const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj)
{
	OPTICK_EVENT();

	auto batch_list = drawable->generate_batch_list(model, view, proj);

	for (const auto& batch : batch_list)
	{
		batches.emplace_back(batch);
	}
}

void Rendy::AbstractEngine::flush()
{
	OPTICK_EVENT();

	//TODO: set default states?
	for (auto& stage : render_stages)
	{
		stage->execute(batches);
	}

	batches.clear();
}

void Rendy::AbstractEngine::reload()
{
	OPTICK_EVENT();

	for (auto& stage: render_stages)
	{
		stage->reload();
	}

	batches.clear();
}

bool Rendy::AbstractEngine::validate()
{
	OPTICK_EVENT();

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
	OPTICK_EVENT();
	return static_cast<uint32_t>(render_stages.size());
}

Rendy::AbstractRenderStageRef Rendy::AbstractEngine::get_stage(uint32_t index)
{
	OPTICK_EVENT();

	assert(static_cast<size_t>(index) < render_stages.size());
	return render_stages[index];
}

void Rendy::AbstractEngine::add_stage(AbstractRenderStageRef stage)
{
	OPTICK_EVENT();
	render_stages.emplace_back(stage);
}
