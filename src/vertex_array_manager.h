#pragma once
#include "singleton.h"
#include "abstract_vertex_array.h"
#include <vector>

class VertexArrayManager: public Singleton<VertexArrayManager>
{
public:
	AbstractVertexArrayRef make(const std::vector<float>& vbo_src,
		const std::vector<uint16_t>& ibo_src,
		const BufferLayoutRef& layout);

	AbstractVertexArrayRef make(const void* vbo_ptr, uint32_t vbo_size,
		const void* ibo_ptr, uint32_t ibo_size, const BufferLayoutRef& layout);

	void reload();
	void flush();
private:
	std::vector<AbstractVertexArrayRef> vao_list;
};