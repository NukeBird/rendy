#include "shader_variant.h"
#include <GL/glew.h> //TODO

ES2::ShaderVariant::ShaderVariant(const std::string& vtx, const std::string& frg)
{
	this->vertex_source = vtx;
	this->fragment_source = frg;
	compile_shader();
}

ES2::ShaderVariant::~ShaderVariant()
{
	reset();
}

void ES2::ShaderVariant::reload()
{
	if (!validate())
	{
		compile_shader();
	}
}

bool ES2::ShaderVariant::validate() const
{
	if (glIsProgram(program_id))
	{
		int link_status;
		glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

		return link_status != GL_FALSE;
	}

	return false;
}

void ES2::ShaderVariant::bind()
{
	glUseProgram(program_id);
}

void ES2::ShaderVariant::unbind()
{
	glUseProgram(0);
}

void ES2::ShaderVariant::compile_shader()
{
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
}

void ES2::ShaderVariant::reset()
{
	glDeleteProgram(program_id);
	program_id = 0;
}
