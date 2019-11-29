#pragma once
#include "../abstract_resource.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractShaderVariant : public AbstractResource
	{
		virtual void set_uniform(const std::string& name, const glm::vec3& vec) = 0;
		virtual void set_uniform(const std::string& name, const glm::mat4& mat) = 0;
		virtual void set_uniform(const std::string& name, const glm::mat3& mat) = 0;
		virtual void set_uniform(const std::string& name, const float v) = 0;
		virtual void set_uniform(const std::string& name, const int v) = 0;
		virtual void set_uniform(const std::string& name, 
			const std::vector<glm::vec3>& vec_array) = 0;
		virtual void set_uniform(const std::string& name, 
			const std::vector<glm::mat4>& mat_array) = 0;
		virtual void set_uniform(const std::string& name, 
			const std::vector<glm::mat3>& mat_array) = 0;
		virtual void set_uniform(const std::string& name, 
			const std::vector<float>& float_array) = 0;
		virtual void set_uniform(const std::string& name, 
			const std::vector<int>& int_array) = 0;
		virtual int get_attribute_location(const std::string& name) const = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	using ShaderVariantRef = std::shared_ptr<AbstractShaderVariant>;
};