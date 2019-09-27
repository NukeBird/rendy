#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <memory>

class IndexBufferManager: public Singleton<IndexBufferManager>
{
public:
	AbstractBufferRef make(uint16_t size, const void* ptr);
	void reload();
	void flush();
private:
	std::vector<AbstractBufferRef> ibo_list;
};