#pragma once
#include "../abstract_shader_variant.h"
#include <string>

namespace ES2
{
	class ShaderVariant : public AbstractShaderVariant
	{
	public:
		ShaderVariant(const std::string& vtx, const std::string& frg);
		~ShaderVariant();
		virtual void reload() override; //TODO: pass sources from AbstractShader
		virtual bool validate() const override;
		virtual void bind() override;
		virtual void unbind() override;
	private:
		void compile_shader();
		void reset();

		std::string vertex_source; //TODO: remove
		std::string fragment_source; //TODO: remove
		uint32_t program_id = 0;
	};
};