#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <memory>
#include <vector>

class IndexBufferManager: public Singleton<IndexBufferManager>
{
public:
	AbstractBufferRef make(uint32_t size, const void* ptr);
	void reload();
	void flush();
private:
	std::vector<AbstractBufferRef> ibo_list;
};