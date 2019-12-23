#include "vertex_array_es3.h"
#include <optick.h>
#include "../util/log.h"

Rendy::ES3::VertexArray::VertexArray(AbstractBufferRef vbo,
	AbstractBufferRef ibo, const BufferLayoutRef& layout)
{
	OPTICK_EVENT();

	this->vbo = vbo;
	this->ibo = ibo;
	this->layout = layout;

	generate_vao();
}

Rendy::ES3::VertexArray::~VertexArray()
{
	OPTICK_EVENT();

	if (glIsVertexArray(id))
	{
		glDeleteVertexArrays(1, &id);
	}
}

void Rendy::ES3::VertexArray::reload()
{
	OPTICK_EVENT();

	last_binded_shader = nullptr;
	generate_vao();

	//won't do anything if vbo/ibo valid
	if (vbo)
	{
		vbo->reload();
	}

	if (ibo)
	{
		ibo->reload();
	}
}

bool Rendy::ES3::VertexArray::validate() const
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

void Rendy::ES3::VertexArray::bind(const ShaderVariantRef& shader)
{
	OPTICK_EVENT();

	glBindVertexArray(id);

	if (shader != last_binded_shader)
	{
		last_binded_shader = shader;
		rebind(shader);
	}
}

void Rendy::ES3::VertexArray::draw()
{
	OPTICK_EVENT();

	if (ibo)
	{
		glDrawElements(GL_TRIANGLES, ibo->get_size() / sizeof(uint32_t),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vbo->get_size() / layout->get_stride());
	}
}

void Rendy::ES3::VertexArray::unbind()
{
	OPTICK_EVENT();
	glBindVertexArray(0);
}

Rendy::BufferLayoutRef Rendy::ES3::VertexArray::get_layout() const
{
	OPTICK_EVENT();
	return layout;
}

Rendy::AbstractBufferRef Rendy::ES3::VertexArray::get_vertex_buffer()
{
	OPTICK_EVENT();
	return vbo;
}

Rendy::AbstractBufferRef Rendy::ES3::VertexArray::get_index_buffer()
{
	OPTICK_EVENT();
	return ibo;
}

void Rendy::ES3::VertexArray::rebind(const ShaderVariantRef& shader)
{
	glBindVertexArray(id);

	if (vbo)
	{
		vbo->bind();
	}

	if (ibo)
	{
		ibo->bind();
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	bind_layout(shader);
}

void Rendy::ES3::VertexArray::generate_vao()
{
	if (!glIsVertexArray(id))
	{
		glGenVertexArrays(1, &id);
	}
}

void Rendy::ES3::VertexArray::bind_layout(const ShaderVariantRef& shader)
{
	OPTICK_EVENT();

	for (const auto& e : (*layout))
	{
		const int index = shader->get_attribute_location(e.name);
		OPTICK_TAG(e.name.c_str(), index);

		if (index >= 0)
		{
			//Log::info("({0}){1}: {2}", (unsigned)this, e.name.c_str(), index);

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, e.get_component_count(), get_gl_type(e), 
				false, layout->get_stride(), reinterpret_cast<const void*>(e.offset)); //TODO warning C4312:  'reinterpret_cast': 
														//conversion from 'const uint32_t' to 'const void *' of greater size
		}
		else
		{
			Log::info("(NOT FOUND) {0}", e.name.c_str());
		}
	}
}

GLenum Rendy::ES3::VertexArray::get_gl_type(const BufferElement& element)
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
