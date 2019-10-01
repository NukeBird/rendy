#include "vertex_array.h"
#include "../vertex_buffer_manager.h"
#include "../index_buffer_manager.h"

ES2::VertexArray::VertexArray(AbstractBufferRef vbo, 
	AbstractBufferRef ibo, const BufferLayoutRef& layout)
{
	this->vbo = vbo;
	this->ibo = ibo;
	this->layout = layout;
}

ES2::VertexArray::VertexArray(const void* vbo_ptr, 
	uint32_t vbo_size, const void* ibo_ptr, 
	uint16_t ibo_size, const BufferLayoutRef& layout):
	VertexArray(VertexBufferManager::get_instance()->make(vbo_size, vbo_ptr),
		IndexBufferManager::get_instance()->make(ibo_size, ibo_ptr), layout) 
{

}

void ES2::VertexArray::reload()
{
	//won't do anything if vbo/ibo valid
	vbo->reload();
	ibo->reload();
}

bool ES2::VertexArray::validate() const
{
	bool is_valid = (vbo != nullptr);

	if(vbo)
	{
		is_valid &= vbo->validate();
	}

	if (ibo)
	{
		is_valid &= ibo->validate();
	}

	is_valid &= (layout != nullptr);

	return is_valid;
}

void ES2::VertexArray::bind()
{
	if (vbo)
	{
		vbo->bind();
	}

	bind_layout();

	if (ibo)
	{
		ibo->bind();
	}
}

void ES2::VertexArray::draw()
{
	//bind();

	if (ibo)
	{
		glDrawElements(GL_TRIANGLES, ibo->get_size() / sizeof(uint16_t),
			GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vbo->get_size() / layout->get_stride());
	}

	//unbind();
}

void ES2::VertexArray::unbind()
{
	if (vbo)
	{
		vbo->unbind();
	}

	if (ibo)
	{
		ibo->unbind();
	}
}

BufferLayoutRef ES2::VertexArray::get_layout() const
{
	return layout;
}

AbstractBufferRef ES2::VertexArray::get_vertex_buffer()
{
	return vbo;
}

AbstractBufferRef ES2::VertexArray::get_index_buffer()
{
	return ibo;
}

void ES2::VertexArray::bind_layout()
{
	uint32_t index = 0;
	for (const auto& e : (*layout))
	{
		///TODO!!!!!!!!!!!!
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, e.get_component_count(), get_gl_type(e), 
			false, layout->get_stride(), reinterpret_cast<const void*>(e.offset)); //warning C4312:  'reinterpret_cast': 
													//conversion from 'const uint32_t' to 'const void *' of greater size
		index++;
	}
}

GLenum ES2::VertexArray::get_gl_type(const BufferElement& element)
{
	switch (element.type)
	{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
	}

	return -1;
}
