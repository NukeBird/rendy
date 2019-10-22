#include "shader_factory.h"
#include "es2/shader.h"
#include <algorithm>
#include "generic_shader_sources.hpp"
#include <optick.h>

AbstractShaderRef ShaderFactory::make(const std::string& vtx, const std::string& frg)
{
	OPTICK_EVENT();

	auto shader = std::make_shared<ES2::Shader>(vtx, frg);
	return shader;
}

constexpr void generate_permutations(uint32_t n, uint32_t max_n, std::vector<uint32_t>& arr,
	uint32_t mask = 0)
{
	if (n > max_n)
	{
		return;
	}

	if (n == 0)
	{
		arr.push_back(mask);
	}

	//arr.push_back(mask);
	generate_permutations(n + 1, max_n, arr, mask);

	mask = mask | (1 << n);
	arr.push_back(mask);

	generate_permutations(n + 1, max_n, arr, mask);
}

std::vector<uint32_t> generate_permutations(uint32_t max_n)
{
	std::vector<uint32_t> result;
	generate_permutations(0, max_n, result);
	return std::move(result);
}

#include <mutex>

AbstractShaderRef ShaderFactory::get_generic_shader()
{
	OPTICK_EVENT();

	if (!generic_shader)
	{
		generic_shader = make(generic_vertex_shader, generic_fragment_shader); //TODO
	}

	/*static std::once_flag flag;
	std::call_once(flag, 
		[this]()
	{
		auto perm = generate_permutations(16);
		for (auto& i: perm)
		{
			ShaderSettings settings;
			settings.flags = i | USE_VERTEX_POSITION;
			auto variant = generic_shader->compile(settings);

			if (!variant->validate())
			{
				printf("[Invalid shader route]:\n%s\n\n", shader_flags_to_defines(settings.flags).c_str());
			}
		}
	});*/

	return generic_shader;
}