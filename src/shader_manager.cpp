#include "shader_manager.h"
#include "es2/shader.h"
#include <algorithm>

AbstractShaderRef ShaderManager::make(const std::string& vtx, const std::string& frg)
{
	auto shader = std::make_shared<ES2::Shader>(vtx, frg);
	shader_list.emplace_back(shader);
	return shader;
}

void ShaderManager::reload()
{
	for (auto& shader : shader_list)
	{
		shader->reload();
	}
}

void ShaderManager::flush()
{
	const auto rule = [](const AbstractShaderRef& shader)
	{
		return shader.use_count() == 1 || !shader->validate();
	};

	auto remove_it = std::remove_if(shader_list.begin(),
		shader_list.end(), rule);

	shader_list.erase(remove_it);
}
