#include "buffer_layout.h"
#include <optick.h>

Rendy::BufferLayout::BufferLayout(const std::vector<BufferElement>& elements)
{
	OPTICK_EVENT();
	this->elements = elements;
	calculate();
}

uint32_t Rendy::BufferLayout::get_stride() const
{
	OPTICK_EVENT();
	OPTICK_TAG("stride", stride);
	return stride;
}

const std::vector<Rendy::BufferElement>& Rendy::BufferLayout::get_elements() const
{
	OPTICK_EVENT();
	return elements;
}

std::vector<Rendy::BufferElement>::iterator Rendy::BufferLayout::begin()
{
	return elements.begin();
}

std::vector<Rendy::BufferElement>::iterator Rendy::BufferLayout::end()
{
	return elements.end();
}

std::vector<Rendy::BufferElement>::const_iterator Rendy::BufferLayout::begin() const
{
	return elements.begin();
}

std::vector<Rendy::BufferElement>::const_iterator Rendy::BufferLayout::end() const
{
	return elements.end();
}

void Rendy::BufferLayout::calculate()
{
	OPTICK_EVENT();
	uint32_t offset = 0;
	stride = 0;

	for (auto& element: elements)
	{
		element.offset = offset;
		offset += element.size;
		stride += element.size;
	}

	OPTICK_TAG("offset", offset);
	OPTICK_TAG("stride", stride);
}

Rendy::BufferElement::BufferElement(ShaderDataType type, const std::string& name)
{
	OPTICK_EVENT();
	this->name = name;
	this->type = type;
	this->size = get_shader_data_type_size(type);
	this->offset = 0;

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("type", to_string(type).c_str());
	OPTICK_TAG("size", size);
}

uint32_t Rendy::BufferElement::get_component_count() const
{
	OPTICK_EVENT();
	switch (type)
	{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat3:    return 3 * 3;
		case ShaderDataType::Mat4:    return 4 * 4;
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 3;
		case ShaderDataType::Int4:    return 4;
		case ShaderDataType::Bool:    return 1;
	}

	return 0;
}