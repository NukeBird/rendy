#pragma once
#include "../abstract_resource.h"
#include "../common.h"
#include "../ogl.hpp"
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractShaderVariant : public AbstractResource
	{
		AbstractShaderVariant(OGL version, const std::string& vtx,
			const std::string& frg);
		virtual ~AbstractShaderVariant();
		void set_uniform(const std::string& name, const glm::vec3& vec);
		void set_uniform(const std::string& name, const glm::mat4& mat);
		void set_uniform(const std::string& name, const glm::mat3& mat);
		void set_uniform(const std::string& name, float v);
		void set_uniform(const std::string& name, const int v);
		void set_uniform(const std::string& name, const std::vector<glm::vec3>& vec_array);
		void set_uniform(const std::string& name, const std::vector<glm::mat4>& mat_array);
		void set_uniform(const std::string& name, const std::vector<glm::mat3>& mat_array);
		void set_uniform(const std::string& name, const std::vector<float>& float_array);
		void set_uniform(const std::string& name, const std::vector<int>& int_array); 
		void reload() override; //TODO: pass sources from AbstractShader
		bool validate() const override;
		int get_uniform_location(const std::string& name) const;
		int get_attribute_location(const std::string& name) const;
		int get_buffer_binding_point(const std::string& name) const;
		void bind();
		void unbind();
	protected:
		virtual void cache_stuff();
		virtual void cache_attribute_locations() = 0;
		virtual void cache_uniform_locations() = 0;
		virtual void cache_buffer_binding_points() {};
		void compile_shader();
		void reset();

		std::string vertex_source; //TODO: remove
		std::string fragment_source; //TODO: remove
		uint32_t program_id = 0;

		std::unordered_map<std::string, int> attribute_cache;
		std::unordered_map<std::string, int> buffer_cache;
		std::unordered_map<std::string, int> uniform_cache;

		const OGL version;
	};

	using ShaderVariantRef = std::shared_ptr<AbstractShaderVariant>;
};