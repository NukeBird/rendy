#include "vertex_array_manager.h"
#include "es2/vertex_array.h"
#include "vertex_buffer_manager.h"
#include "index_buffer_manager.h"
#include <algorithm>
#include <memory>

AbstractVertexArrayRef VertexArrayManager::make(const std::vector<float>& vbo_src, 
	const std::vector<uint16_t>& ibo_src, const BufferLayoutRef & layout)
{
	const uint32_t vbo_size = vbo_src.size() * sizeof(float);
	const void* vbo_ptr = &vbo_src[0];

	const uint32_t ibo_size = ibo_src.size() * sizeof(uint16_t);
	const void* ibo_ptr = &ibo_src[0];

	return make(vbo_ptr, vbo_size, ibo_ptr, ibo_size, layout);
}

AbstractVertexArrayRef VertexArrayManager::make(const void* vbo_ptr, uint32_t vbo_size,
	const void* ibo_ptr, uint32_t ibo_size, const BufferLayoutRef& layout)
{
	return std::make_shared<ES2::VertexArray>(
		VertexBufferManager::get_instance()->make(vbo_size, vbo_ptr),
		IndexBufferManager::get_instance()->make(ibo_size, ibo_ptr), layout);
}

void VertexArrayManager::reload()
{
	for (auto& vao : vao_list)
	{
		vao->reload();
	}
}

void VertexArrayManager::flush()
{
	const auto rule = [](const AbstractVertexArrayRef& vbo)
	{
		return vbo.use_count() == 1 || !vbo->validate();
	};

	auto remove_it = std::remove_if(vao_list.begin(),
		vao_list.end(), rule);

	vao_list.erase(remove_it);
}
