#pragma once
#include "../abstract_shader_variant.h"
#include <unordered_map>
#include <string>

namespace Rendy
{
	namespace ES2
	{
		class ShaderVariant : public AbstractShaderVariant
		{
		public:
			ShaderVariant(const std::string& vtx, const std::string& frg);
			~ShaderVariant();
			virtual void reload() override; //TODO: pass sources from AbstractShader
			virtual bool validate() const override;
			virtual void set_uniform(const std::string& name, const glm::vec3& vec) override;
			virtual void set_uniform(const std::string& name, const glm::mat4& mat) override;
			virtual void set_uniform(const std::string& name, const glm::mat3& mat) override;
			virtual void set_uniform(const std::string& name, const float number) override;
			virtual void set_uniform(const std::string& name, const int number) override;
			virtual void set_uniform(const std::string& name, 
				const std::vector<glm::vec3>& vec_array) override;
			virtual void set_uniform(const std::string& name,
				const std::vector<glm::mat4>& mat_array) override;
			virtual void set_uniform(const std::string& name,
				const std::vector<glm::mat3>& mat_array) override;
			virtual void set_uniform(const std::string& name,
				const std::vector<float>& float_array) override;
			virtual void set_uniform(const std::string& name,
				const std::vector<int>& int_array) override;
			virtual int get_attribute_location(const std::string& name) const override;
			virtual void bind() override;
			virtual void unbind() override;
		private:
			int get_uniform_location(const std::string& name) const;
			void cache_attribute_locations();
			void cache_uniform_locations();
			void compile_shader();
			void reset();

			std::string vertex_source; //TODO: remove
			std::string fragment_source; //TODO: remove
			uint32_t program_id = 0;

			std::unordered_map<std::string, int> attribute_cache;
			std::unordered_map<std::string, int> uniform_cache;
		};
	};
};