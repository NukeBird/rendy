#include "shader_variant.h"
#include <GL/glew.h> //TODO
#include <optick.h>

ES2::ShaderVariant::ShaderVariant(const std::string& vtx, const std::string& frg)
{
	OPTICK_EVENT();

	this->vertex_source = vtx;
	this->fragment_source = frg;
	compile_shader();

	OPTICK_TAG("id", program_id);
}

ES2::ShaderVariant::~ShaderVariant()
{
	OPTICK_EVENT();

	reset();
}

void ES2::ShaderVariant::reload()
{
	OPTICK_EVENT();

	if (!validate())
	{
		compile_shader();
	}
}

bool ES2::ShaderVariant::validate() const
{
	OPTICK_EVENT();

	if (glIsProgram(program_id))
	{
		int link_status;
		glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

		return link_status != GL_FALSE;
	}

	return false;
}

void ES2::ShaderVariant::set_uniform(const std::string& name, const glm::vec3& vec)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location == -1)
	{
		//TODO: warning
	}
	else
	{
		glUniform3fv(location, 1, &vec[0]);
	}
}

void ES2::ShaderVariant::set_uniform(const std::string& name, const glm::mat4& mat)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location == -1)
	{
		//TODO: warning
	}
	else
	{
		glUniformMatrix4fv(location, 1, false, &mat[0][0]);
	}
}

void ES2::ShaderVariant::set_uniform(const std::string& name, const glm::mat3& mat)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location == -1)
	{
		//TODO: warning
	}
	else
	{
		glUniformMatrix3fv(location, 1, false, &mat[0][0]);
	}
}

void ES2::ShaderVariant::set_uniform(const std::string& name, const float number)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location == -1)
	{
		//TODO: warning
	}
	else
	{
		glUniform1fv(location, 1, &number);
	}
}

void ES2::ShaderVariant::set_uniform(const std::string& name, const int number)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location == -1)
	{
		//TODO: warning
	}
	else
	{
		glUniform1iv(location, 1, &number);
	}
}

int ES2::ShaderVariant::get_attribute_location(const std::string& name) const
{
	OPTICK_EVENT();

	int location = -1;
	auto it = attribute_cache.find(name);

	if (it != attribute_cache.end())
	{
		return (*it).second;
	}

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("location", location);

	return location;
}

void ES2::ShaderVariant::bind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glUseProgram(program_id);
}

void ES2::ShaderVariant::unbind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glUseProgram(0);
}

int ES2::ShaderVariant::get_uniform_location(const std::string& name) const
{
	OPTICK_EVENT();

	int location = -1;
	auto it = uniform_cache.find(name);

	if (it != uniform_cache.end())
	{
		return (*it).second;
	}

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("location", location);

	return location;
}

void ES2::ShaderVariant::cache_attribute_locations()
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

void ES2::ShaderVariant::cache_uniform_locations()
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
}

void ES2::ShaderVariant::compile_shader()
{
	OPTICK_EVENT();

	reset();
	uint32_t vtx_id = glCreateShader(GL_VERTEX_SHADER);
	const char* vtx_str = vertex_source.c_str();
	glShaderSource(vtx_id, 1, &vtx_str, NULL);
	glCompileShader(vtx_id);
	uint32_t frg_id = glCreateShader(GL_FRAGMENT_SHADER);
	const char* frg_str = fragment_source.c_str();
	glShaderSource(frg_id, 1, &frg_str, NULL);
	glCompileShader(frg_id);

	program_id = glCreateProgram();
	glAttachShader(program_id, vtx_id);
	glAttachShader(program_id, frg_id);
	glLinkProgram(program_id);

	glDeleteShader(vtx_id);
	glDeleteShader(frg_id);

	cache_attribute_locations();
	cache_uniform_locations();

	OPTICK_TAG("id", program_id);
}

void ES2::ShaderVariant::reset()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glDeleteProgram(program_id);
	program_id = 0;
}
