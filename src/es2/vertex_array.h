#pragma once
#include "../abstract_vertex_array.h"
#include "../common.h"

namespace Rendy
{
	namespace ES2
	{
		class VertexArray : public AbstractVertexArray
		{
		public:
			VertexArray(AbstractBufferRef vbo, AbstractBufferRef ibo,
				const BufferLayoutRef& layout);
			virtual void reload() override;
			virtual bool validate() const override;
			virtual void bind(const ShaderVariantRef& shader) override;
			virtual void draw() override; //TODO: move to another class?
			virtual void unbind() override;
			virtual BufferLayoutRef get_layout() const override;
			virtual AbstractBufferRef get_vertex_buffer() override;
			virtual AbstractBufferRef get_index_buffer() override;
		private:
			void bind_layout(const ShaderVariantRef& shader);

			GLenum get_gl_type(const BufferElement& element);

			BufferLayoutRef layout = nullptr;
			AbstractBufferRef vbo = nullptr;
			AbstractBufferRef ibo = nullptr;
		};
	};
};