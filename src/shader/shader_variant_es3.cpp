#include "shader_variant_es3.h"
#include "../common.h"
#include "../util/log.h"
#include <optick.h>

Rendy::ES3::ShaderVariant::ShaderVariant(const std::string& vtx, 
	const std::string& frg): AbstractShaderVariant(OGL::ES31, vtx, frg)
{
	OPTICK_EVENT();
	cache_stuff();
}

void Rendy::ES3::ShaderVariant::cache_attribute_locations()
{
	OPTICK_EVENT();
	assert(program_id > 0);

	int attribute_count, attribute_name_max_len;
	glGetProgramInterfaceiv(program_id, GL_PROGRAM_INPUT,
		GL_ACTIVE_RESOURCES, &attribute_count);
	glGetProgramInterfaceiv(program_id, GL_PROGRAM_INPUT,
		GL_MAX_NAME_LENGTH, &attribute_name_max_len);

	std::string name_buffer;
	name_buffer.resize(attribute_name_max_len);

	for (int i = 0; i < attribute_count; i++) {

		GLsizei str_length;
		glGetProgramResourceName(program_id, GL_PROGRAM_INPUT,
			i, attribute_name_max_len, &str_length, &name_buffer[0]);

		// get resource index of the shader storage block
		GLint location = glGetProgramResourceLocation(program_id,
			GL_PROGRAM_INPUT, &name_buffer[0]);

		const std::string name = name_buffer.substr(0, str_length);

		attribute_cache[name] = location;
	}
}

void Rendy::ES3::ShaderVariant::cache_uniform_locations()
{
	OPTICK_EVENT();

	assert(program_id > 0);

	int uniform_count, uniform_name_max_len;
	glGetProgramInterfaceiv(program_id, GL_UNIFORM, 
		GL_ACTIVE_RESOURCES, &uniform_count);
	glGetProgramInterfaceiv(program_id, GL_UNIFORM,
		GL_MAX_NAME_LENGTH, &uniform_name_max_len);

	std::string name_buffer;
	name_buffer.resize(uniform_name_max_len);

	for (int i = 0; i < uniform_count; i++) {

		GLsizei str_length;
		glGetProgramResourceName(program_id, GL_UNIFORM,
			i, uniform_name_max_len, &str_length, &name_buffer[0]);

		// get resource index of the shader storage block
		GLint location = glGetProgramResourceLocation(program_id,
			GL_UNIFORM, &name_buffer[0]);

		const std::string name = name_buffer.substr(0, str_length);
		
		uniform_cache[name] = location;
	}
}
