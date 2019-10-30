#pragma once
#include "../abstract_engine.h"

namespace Rendy
{
	namespace ES2
	{
		class Engine final: public AbstractEngine
		{
		public:
			virtual void push(BatchList batches) override;
			virtual void push(CommandList commands) override;
			virtual void flush() override;
			virtual void reload() override;

			virtual AbstractShaderRef make_shader(const std::string& vtx,
				const std::string& frg) override;
			virtual AbstractTexture2DRef make_texture2d(Image2DRef image) override;
			virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
				BufferLayoutRef layout) override;
			virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) override;
			virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) override;
			virtual IndexType get_index_type() const override;
		};
	};
};