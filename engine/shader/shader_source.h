#pragma once
#include "shader_type.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace Rendy
{
	class ShaderSource;

	using ShaderSourceRef = std::shared_ptr<ShaderSource>;

	class ShaderSource final
	{
	public:
		ShaderSource() = default;
		ShaderSource(const std::string& vtx, const std::string& frg);
		void set_source(ShaderType type, const std::string& source);
		const std::string& get_source(ShaderType type);
		ShaderSourceRef combine(ShaderSourceRef source);
		ShaderSourceRef combine(ShaderSource& source);
		bool operator==(ShaderSource& s);
	private:
		std::unordered_map<ShaderType, std::string> sources;
	};
};