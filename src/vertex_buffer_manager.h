#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <vector>

class VertexBufferManager: public Singleton<VertexBufferManager>
{
public:
	AbstractBufferRef make(uint32_t size, const void* ptr);
	void reload();
	void flush();
private:
	std::vector<AbstractBufferRef> vbo_list;
};