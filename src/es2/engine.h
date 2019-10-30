#pragma once
#include "../abstract_engine.h"
#include "../abstract_vfs.h"

namespace Rendy
{
	namespace ES2
	{
		class Engine final: public AbstractEngine
		{
		public:
			Engine(VFSRef vfs);

			virtual void push(BatchList batches) override;
			virtual void push(CommandList commands) override;
			virtual void flush() override;
			virtual void reload() override;

			virtual AbstractShaderRef make_shader(const std::string& vtx,
				const std::string& frg) override;
			virtual AbstractMaterialRef make_material(ImageSetRef image_set) override;
			virtual AbstractTexture2DRef make_texture2d(Image2DRef image) override;
			virtual AbstractTextureCubeRef make_texture_cube(uint32_t size, const void* ptr) override;
			virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, AbstractBufferRef ibo,
				BufferLayoutRef layout) override;
			virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) override;
			virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr) override;
			virtual IndexType get_index_type() const override;
		private:
			AbstractShaderRef generic_shader;
			AbstractTextureCubeRef iem;
			VFSRef vfs;
		};
	};
};