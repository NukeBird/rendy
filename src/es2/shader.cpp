#include "shader.h"
#include "shader_variant.h"
#include <GL/glew.h> //TODO

ES2::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source)
{
	this->vertex_source = vertex_source;
	this->fragment_source = fragment_source;

	compile(get_all_shader_flags());
}

void ES2::Shader::reload()
{
	for (auto& v : variants)
	{
		v.second->reload();
	}
}

ShaderVariantRef ES2::Shader::compile(uint32_t flags)
{
	auto it = variants.find(flags);

	if (it == variants.end())
	{
		const std::string meta = "#version 330 core\n" //TODO
			+ shader_flags_to_defines(flags);
		const std::string vertex_source_variant = meta + vertex_source;
		const std::string fragment_source_variant = meta + fragment_source;
		variants[flags] = std::make_shared<ShaderVariant>(vertex_source_variant,
			fragment_source_variant);
			
		it = variants.find(flags);
	}

	return it->second;
}

bool ES2::Shader::validate() const
{
	for (auto& v : variants)
	{
		if (!v.second->validate())
		{
			return false;
		}
	}

	return true;
}

uint32_t ES2::Shader::get_variant_count() const
{
	return static_cast<uint32_t>(variants.size());
}
