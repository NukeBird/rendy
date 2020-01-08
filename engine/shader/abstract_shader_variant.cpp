#include "abstract_shader_variant.h"
#include "../util/log.h"
#include <optick.h>

Rendy::AbstractShaderVariant::AbstractShaderVariant(OGL version,
	const std::string& vtx, const std::string& frg): version(version)
{
	OPTICK_EVENT();

	this->vertex_source = vtx;
	this->fragment_source = frg;
	compile_shader();

	OPTICK_TAG("id", program_id);
}

Rendy::AbstractShaderVariant::~AbstractShaderVariant()
{
	OPTICK_EVENT();
	reset();
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name, const glm::vec3& vec)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform3fv(location, 1, &vec[0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name, const glm::mat4& mat)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &mat[0][0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name, const glm::mat3& mat)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniformMatrix3fv(location, 1, false, &mat[0][0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name, const float number)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform1fv(location, 1, &number);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name, const int number)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform1iv(location, 1, &number);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name,
	const std::vector<glm::vec3>& vec_array)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform3fv(location, static_cast<GLsizei>(vec_array.size()),
			&vec_array[0][0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name,
	const std::vector<glm::mat4>& mat_array)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniformMatrix4fv(location, static_cast<GLsizei>(mat_array.size()),
			false, glm::value_ptr(mat_array[0]));
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name,
	const std::vector<glm::mat3>& mat_array)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniformMatrix3fv(location, static_cast<GLsizei>(mat_array.size()),
			false, &mat_array[0][0][0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name,
	const std::vector<float>& float_array)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform1fv(location, static_cast<GLsizei>(float_array.size()),
			&float_array[0]);
	}
}

void Rendy::AbstractShaderVariant::set_uniform(const std::string& name,
	const std::vector<int>& int_array)
{
	OPTICK_EVENT();
	OPTICK_TAG("name", name.c_str());

	auto location = get_uniform_location(name);

	if (location != -1)
	{
		glUniform1iv(location, static_cast<GLsizei>(int_array.size()),
			&int_array[0]);
	}
}


int Rendy::AbstractShaderVariant::get_attribute_location(const std::string& name) const
{
	OPTICK_EVENT();

	int location = -1;
	auto it = attribute_cache.find(name);

	if (it != attribute_cache.end())
	{
		location = (*it).second;
	}

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("location", location);

	return location;
}

int Rendy::AbstractShaderVariant::get_buffer_binding_point(const std::string & name) const
{
	OPTICK_EVENT();

	int location = -1;
	auto it = buffer_cache.find(name);

	if (it != buffer_cache.end())
	{
		location = (*it).second;
	}

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("location", location);

	return location;
}

void Rendy::AbstractShaderVariant::bind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glUseProgram(program_id);
}

void Rendy::AbstractShaderVariant::unbind()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glUseProgram(0);
}

void Rendy::AbstractShaderVariant::cache_stuff()
{
	cache_uniform_locations();
	cache_attribute_locations();
	cache_buffer_binding_points();

	if (!attribute_cache.empty())
	{
		Log::info("CACHED ATTRIBUTES");
		for (auto& i : attribute_cache)
		{
			Log::info("{0}: {1}", i.first, i.second);
		}
	}

	if (!uniform_cache.empty())
	{
		Log::info("CACHED UNIFORMS");
		for (auto& i : uniform_cache)
		{
			Log::info("{0}: {1}", i.first, i.second);
		}
	}

	if (!buffer_cache.empty())
	{
		Log::info("BUFFER BINDING POINTS");
		for (auto& i : buffer_cache)
		{
			Log::info("{0}: {1}", i.first, i.second);
		}
	}
}

void Rendy::AbstractShaderVariant::reload()
{
	OPTICK_EVENT();

	if (!validate())
	{
		compile_shader();
	}
}

bool Rendy::AbstractShaderVariant::validate() const
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

int Rendy::AbstractShaderVariant::get_uniform_location(const std::string& name) const
{
	OPTICK_EVENT();

	int location = -1;
	auto it = uniform_cache.find(name);

	if (it != uniform_cache.end())
	{
		location = (*it).second;
	}

	OPTICK_TAG("name", name.c_str());
	OPTICK_TAG("location", location);

	return location;
}

void Rendy::AbstractShaderVariant::compile_shader()
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

	OPTICK_TAG("id", program_id);
}

void Rendy::AbstractShaderVariant::reset()
{
	OPTICK_EVENT();
	OPTICK_TAG("id", program_id);

	glDeleteProgram(program_id);
	program_id = 0;
}
