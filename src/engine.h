#pragma once
#include "ogl.hpp"
#include "gapi.h"
#include "image/image_set.h"
#include "pipeline/pipeline.h"
#include "material/material_factory.h"
#include "util/log.h"
#include <vector>
#include <string>
#include <memory>

namespace Rendy
{
	class Engine final
	{
	public:
		Engine(OGL version, VFSRef vfs);
		void push(AbstractDrawableRef drawable, const glm::mat4& model,
			const glm::mat4& view, const glm::mat4& proj);
		void flush();
		void reload();
		bool validate();
		GAPIRef get_gapi() const;
		PipelineRef get_pipeline() const;
		void set_pipeline(PipelineRef pipeline);
		AbstractShaderRef make_shader(const std::string& vtx, 
			const std::string& frg);
		AbstractShaderRef make_shader(ShaderSourceRef source);
		AbstractMaterialRef make_material(ImageSetRef image_set);
		AbstractTexture2DRef make_texture2d(Image2DRef image);
		AbstractTextureCubeRef make_texture_cube(uint32_t size, const void* ptr);
		AbstractTextureCubeRef make_texture_cube(const std::string& path);
		AbstractVertexArrayRef make_vao(AbstractBufferRef vbo, 
			AbstractBufferRef ibo, BufferLayoutRef layout);
		AbstractBufferRef make_vbo(uint32_t size, const void* ptr);
		AbstractBufferRef make_ibo(uint32_t size, const void* ptr);
		AbstractBufferRef make_sbo(uint32_t size, const void* ptr);
		AbstractBufferRef make_cbo(uint32_t size, const void* ptr);
		IndexType get_index_type() const; 
	private:
		GAPIRef gapi;
		AbstractTextureCubeRef iem;
		AbstractTextureCubeRef pmrem;
		AbstractTexture2DRef lut;
		MaterialFactoryRef material_factory;
		VFSRef vfs;

		PipelineRef pipeline;
		BatchList batches;
		OGL version;
	};

	using EngineRef = std::shared_ptr<Engine>;
};