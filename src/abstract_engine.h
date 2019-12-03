#pragma once
#include "gapi.h"
#include "image/image_set.h"
#include "pipeline/pipeline.h"
#include <vector>
#include <string>
#include <memory>

namespace Rendy
{
	struct AbstractEngine
	{
		virtual void push(AbstractDrawableRef drawable, const glm::mat4& model,
			const glm::mat4& view, const glm::mat4& proj);
		virtual void flush();
		virtual void reload();
		virtual bool validate();
		
		virtual GAPIRef get_gapi() const = 0;
		PipelineRef get_pipeline() const;
		void set_pipeline(PipelineRef pipeline);

		virtual AbstractShaderRef make_shader(const std::string& vtx, 
			const std::string& frg);
		virtual AbstractMaterialRef make_material(ImageSetRef image_set) = 0;
		virtual AbstractTexture2DRef make_texture2d(Image2DRef image);
		virtual AbstractTextureCubeRef make_texture_cube(uint32_t size, 
			const void* ptr);
		virtual AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
			AbstractBufferRef ibo, BufferLayoutRef layout);
		virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr);
		virtual AbstractBufferRef make_ibo(uint32_t size, const void* ptr);
		virtual IndexType get_index_type() const;
	private:
		PipelineRef pipeline;
		BatchList batches;
	};

	using AbstractEngineRef = std::shared_ptr<AbstractEngine>;
};