#pragma once
#include "../ogl.hpp"
#include "abstract_shader.h"
#include <memory>
#include <string>

namespace Rendy
{
	class ShaderFactory final
	{
	public:
		ShaderFactory(OGL version);
		AbstractShaderRef make(const std::string& vertex_source,
			const std::string& fragment_source);
	private:
		OGL version;
	};
};