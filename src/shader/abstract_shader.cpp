#include "abstract_shader.h"
#include <optick.h>

Rendy::AbstractShader::AbstractShader(ShaderSourceRef source)
{
	OPTICK_EVENT();
	this->source = source;
}

Rendy::AbstractShader::AbstractShader(const std::string& vertex_shader, 
	const std::string& fragment_shader)
{
	OPTICK_EVENT();

	source = std::make_shared<ShaderSource>();
	source->set_source(ShaderType::VertexShader, vertex_shader);
	source->set_source(ShaderType::FragmentShader, fragment_shader);
}

void Rendy::AbstractShader::reload()
{
	OPTICK_EVENT();

	for (auto& v : variants)
	{
		v.second->reload();
	}
}

bool Rendy::AbstractShader::validate() const
{
	OPTICK_EVENT();

	for (auto& v : variants)
	{
		if (!v.second->validate())
		{
			return false;
		}
	}

	return true;
}

Rendy::ShaderVariantRef Rendy::AbstractShader::compile(const ShaderSettings& settings)
{
	OPTICK_EVENT();

	auto it = variants.find(settings);

	if (it == variants.end())
	{
		const std::string meta = generate_meta(settings);
		const std::string vertex_source_variant = meta + 
			source->get_source(ShaderType::VertexShader);
		const std::string fragment_source_variant = meta +
			source->get_source(ShaderType::FragmentShader);

		variants[settings] = make_variant(vertex_source_variant,
			fragment_source_variant);

		it = variants.find(settings);
	}

	return it->second;
}

uint32_t Rendy::AbstractShader::get_variant_count() const
{
	OPTICK_EVENT();
	return static_cast<uint32_t>(variants.size());
}
