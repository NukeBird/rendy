#include "vertex_array.h"
#include <optick.h>

ES2::VertexArray::VertexArray(AbstractBufferRef vbo, 
	AbstractBufferRef ibo, const BufferLayoutRef& layout)
{
	OPTICK_EVENT();

	this->vbo = vbo;
	this->ibo = ibo;
	this->layout = layout;
}

void ES2::VertexArray::reload()
{
	OPTICK_EVENT();

	//won't do anything if vbo/ibo valid
	vbo->reload();
	ibo->reload();
}

bool ES2::VertexArray::validate() const
{
	OPTICK_EVENT();

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

void ES2::VertexArray::bind(const ShaderVariantRef& shader)
{
	OPTICK_EVENT();

	if (vbo)
	{
		vbo->bind();
	}

	bind_layout(shader);

	if (ibo)
	{
		ibo->bind();
	}
}

void ES2::VertexArray::draw()
{
	OPTICK_EVENT();

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
	OPTICK_EVENT();

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
	OPTICK_EVENT();
	return layout;
}

AbstractBufferRef ES2::VertexArray::get_vertex_buffer()
{
	OPTICK_EVENT();
	return vbo;
}

AbstractBufferRef ES2::VertexArray::get_index_buffer()
{
	OPTICK_EVENT();
	return ibo;
}

void ES2::VertexArray::bind_layout(const ShaderVariantRef& shader)
{
	OPTICK_EVENT();

	for (const auto& e : (*layout))
	{
		const int index = shader->get_attribute_location(e.name);
		OPTICK_TAG(e.name.c_str(), index);

		if (index >= 0)
		{
			//printf("(%p) %s : %d\n", this, e.name.c_str(), index);

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, e.get_component_count(), get_gl_type(e), 
				false, layout->get_stride(), reinterpret_cast<const void*>(e.offset)); //TODO warning C4312:  'reinterpret_cast': 
														//conversion from 'const uint32_t' to 'const void *' of greater size
		}
	}
}

GLenum ES2::VertexArray::get_gl_type(const BufferElement& element)
{
	OPTICK_EVENT();

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
