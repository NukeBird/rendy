#include "vertex_buffer_manager.h"
#include "es2/buffer.h"
#include <algorithm>

AbstractBufferRef VertexBufferManager::make(uint32_t size, const void* ptr)
{
	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto vbo = std::make_shared<ES2::Buffer>(BufferTarget::VBO, size, ptr);
	vbo_list.emplace_back(vbo);
	return vbo;
}

void VertexBufferManager::reload()
{
	for (auto& vbo: vbo_list)
	{
		vbo->reload();
	}
}

void VertexBufferManager::flush()
{
	const auto rule = [](const AbstractBufferRef& vbo)
	{
		return vbo.use_count() == 1 || !vbo->validate();
	};

	auto remove_it = std::remove_if(vbo_list.begin(),
		vbo_list.end(), rule);

	vbo_list.erase(remove_it);
}
