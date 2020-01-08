#include "shader_variant_es2.h"
#include "../common.h"
#include "../util/log.h"
#include <optick.h>

Rendy::ES2::ShaderVariant::ShaderVariant(const std::string& vtx, 
	const std::string& frg): AbstractShaderVariant(OGL::ES20, vtx, frg)
{
	OPTICK_EVENT();
	cache_stuff();
}

void Rendy::ES2::ShaderVariant::cache_attribute_locations()
{
	OPTICK_EVENT();

	assert(program_id > 0);

	int count = 0;
	glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &count);

	GLint size;
	GLenum type;
	const GLsizei buffer_size = 32;
	GLchar name[buffer_size];
	GLsizei length;

	for (int i = 0; i < count; ++i)
	{
		glGetActiveAttrib(program_id, (GLuint)i, buffer_size, &length, &size, &type, name);

		int location = glGetAttribLocation(program_id, name);
		attribute_cache[std::string(name, length)] = location;
	}
}

void Rendy::ES2::ShaderVariant::cache_uniform_locations()
{
	OPTICK_EVENT();

	assert(program_id > 0);

	int count = 0;
	glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);

	GLint size;
	GLenum type;
	const GLsizei buffer_size = 32;
	GLchar name[buffer_size];
	GLsizei length;

	for (int i = 0; i < count; ++i)
	{
		glGetActiveUniform(program_id, (GLuint)i, buffer_size, &length, &size, &type, name);

		int location = glGetUniformLocation(program_id, name);
		uniform_cache[std::string(name, length)] = location;
	}

	Log::info("CACHED UNIFORMS");
	for (auto& i : uniform_cache)
	{
		Log::info("{0} : {1}", i.first, i.second);
	}
}