#pragma once
#include "abstract_resource.h"
#include <memory>
#include <string>

enum class BufferTarget
{
	VBO,
	IBO,
	Invalid
};

std::string to_string(BufferTarget target);

struct AbstractBuffer: public AbstractResource
{
	virtual ~AbstractBuffer() = default;
	virtual void set_data(uint32_t size, const void* ptr) = 0;
	virtual uint32_t get_size() const = 0;
	virtual BufferTarget get_target() const = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};

using AbstractBufferRef = std::shared_ptr<AbstractBuffer>;