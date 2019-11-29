#pragma once
#include "shader/abstract_shader.h"
#include "abstract_command.h"

namespace Rendy
{
	class BindShader : public AbstractCommand
	{
	public:
		BindShader(AbstractShaderRef shader, ShaderSettings settings);
		virtual void execute() override;
	private:
		AbstractShaderRef shader;
		ShaderSettings settings;
	};

	using BindShaderRef = std::shared_ptr<BindShader>;
};