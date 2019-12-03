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

Rendy::AbstractRenderPassRef Rendy::AbstractEngine::get_stage(uint32_t index)
{
	OPTICK_EVENT();

	assert(static_cast<size_t>(index) < render_stages.size());
	return render_stages[index];
}

void Rendy::AbstractEngine::add_stage(AbstractRenderPassRef stage)
{
	OPTICK_EVENT();
	render_stages.emplace_back(stage);
}

Rendy::AbstractShaderRef Rendy::AbstractEngine::make_shader(const std::string & vtx, const std::string & frg)
{
	OPTICK_EVENT();
	return get_gapi()->make_shader(vtx, frg);
}

Rendy::AbstractTexture2DRef Rendy::AbstractEngine::make_texture2d(Image2DRef image)
{
	OPTICK_EVENT();
	return get_gapi()->make_texture2d(image);
}

Rendy::AbstractTextureCubeRef Rendy::AbstractEngine::make_texture_cube(uint32_t size, const void * ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_texture_cube(size, ptr);
}

Rendy::AbstractVertexArrayRef Rendy::AbstractEngine::make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo, BufferLayoutRef layout)
{
	OPTICK_EVENT();
	return get_gapi()->make_vao(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::AbstractEngine::make_vbo(uint32_t size, const void * ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_vbo(size, ptr);
}

Rendy::AbstractBufferRef Rendy::AbstractEngine::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	return get_gapi()->make_ibo(size, ptr);
}

Rendy::IndexType Rendy::AbstractEngine::get_index_type() const
{
	OPTICK_EVENT();
	return get_gapi()->get_index_type();
}
