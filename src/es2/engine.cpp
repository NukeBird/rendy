#include "engine.h"
#include "buffer.h"
#include "vertex_array.h"
#include <optick.h>
#include <cassert>

void Rendy::ES2::Engine::push(BatchList batches)
{
	OPTICK_EVENT();
	CommandList commands;

	for (auto& batch: batches)
	{
		auto cl = batch.to_command_list();

		//TODO: optimize command list!

		commands.insert(commands.end(), cl.begin(), cl.end());
	}

	push(commands);
}

void Rendy::ES2::Engine::push(CommandList commands)
{
	OPTICK_EVENT();
	for (auto& c : commands)
	{
		c->execute(); //TODO
	}
}

void Rendy::ES2::Engine::flush()
{
	OPTICK_EVENT();
	//TODO
}

void Rendy::ES2::Engine::reload()
{
	OPTICK_EVENT();
	//TODO?
}

Rendy::AbstractVertexArrayRef Rendy::ES2::Engine::make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
	BufferLayoutRef layout)
{
	assert(vbo);
	assert(vbo->get_target() == BufferTarget::VBO); //TODO

	if (ibo)
	{
		assert(ibo->get_target() == BufferTarget::IBO); //TODO
	}

	return std::make_shared<VertexArray>(vbo, ibo, layout);
}

Rendy::AbstractBufferRef Rendy::ES2::Engine::make_vbo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	OPTICK_TAG("size", size);

	auto vbo = std::make_shared<ES2::Buffer>(BufferTarget::VBO, size, ptr);
	return vbo;
}

Rendy::AbstractBufferRef Rendy::ES2::Engine::make_ibo(uint32_t size, const void* ptr)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", size);

	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES2::Buffer>(BufferTarget::IBO, size, ptr);
	return ibo;
}

Rendy::IndexType Rendy::ES2::Engine::get_index_type() const
{
	OPTICK_EVENT();
	return IndexType::UnsignedShort;
}
