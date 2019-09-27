#include "index_buffer_manager.h"
#include "es2/buffer.h"
#include <algorithm>
#include <cassert>

AbstractBufferRef IndexBufferManager::make(uint16_t size, const void* ptr)
{
	if (size == 0 || ptr == nullptr)
	{
		return nullptr;
	}

	auto ibo = std::make_shared<ES2::Buffer>(BufferTarget::IBO, size, ptr);
	ibo_list.emplace_back(ibo);
	return ibo;
}

void IndexBufferManager::reload()
{
	for (auto& ibo : ibo_list)
	{
		ibo->reload();
	}
}

void IndexBufferManager::flush()
{
	const auto rule = [](const AbstractBufferRef& vbo)
	{
		return vbo.use_count() == 1 || !vbo->validate();
	};

	auto remove_it = std::remove_if(ibo_list.begin(),
		ibo_list.end(), rule);

	ibo_list.erase(remove_it);
}
