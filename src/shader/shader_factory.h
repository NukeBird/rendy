#pragma once
#include "../ogl.hpp"
#include "abstract_shader.h"
#include <memory>
#include <string>
#include <map>

namespace Rendy
{
	class ShaderFactory final
	{
	public:
		ShaderFactory(OGL version);
		AbstractShaderRef make(ShaderSourceRef source);
		AbstractShaderRef make(const std::string& vtx, const std::string& frg);
	private:
		AbstractShaderRef find_in_cache(ShaderSourceRef source);

		std::map<ShaderSourceRef, AbstractShaderRef> shader_map;
		OGL version;
	};

	using ShaderFactoryRef = std::shared_ptr<ShaderFactory>;
};