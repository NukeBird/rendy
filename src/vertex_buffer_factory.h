#pragma once
#include "singleton.h"
#include "abstract_buffer.h"
#include <vector>

class VertexBufferFactory: public Singleton<VertexBufferFactory>
{
public:
	AbstractBufferRef make(uint32_t size, const void* ptr);
};