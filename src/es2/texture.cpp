#include "texture.h"
#include <GL/glew.h> //TODO
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <mutex> //call_once
#include <optick.h>

ES2::Texture::Texture(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	glGenTextures(1, &id);
	OPTICK_TAG("id", id);
	OPTICK_TAG("size", length);

	this->length = length;

	if (!load_from_memory(memory, length))
	{
		reset();
	}
}

ES2::Texture::~Texture()
{
	OPTICK_EVENT();
	reset();
}

void ES2::Texture::reload()
{
	OPTICK_EVENT();
	if (!validate())
	{
		if (!load_from_memory(static_cast<const char*>(data), length))
		{
			reset();
		}
	}
}

bool ES2::Texture::validate() const
{
	OPTICK_EVENT();
	if (!data)
	{
		return false;
	}

	if (length == 0)
	{
		return false;
	}

	if (id == 0)
	{
		return false;
	}

	if (type == TextureType::Invalid)
	{
		return false;
	}

	if (format == TextureFormat::Invalid)
	{
		return false;
	}

	if (size.x == 0 || size.y == 0) //|| size.z == 0) TODO
	{
		return false;
	}

	if (!glIsTexture(id))
	{
		return false;
	}

	return true;
}

void ES2::Texture::bind(uint32_t slot)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("slot", slot);

	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //TODO

#ifdef _WIN32
	static std::once_flag flag;
	static float max_aniso = 0.0f;
	std::call_once(flag, [&]()
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
		printf("MAX_ANISOTROPY : %f\n", max_aniso);
	});

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
#endif
}

void ES2::Texture::unbind(uint32_t slot)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("slot", slot);

	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::uvec3 ES2::Texture::get_size() const
{
	OPTICK_EVENT();
	return size;
}

TextureFormat ES2::Texture::get_format() const
{
	OPTICK_EVENT();
	return format;
}

TextureType ES2::Texture::get_type() const
{
	OPTICK_EVENT();
	return type;
}

bool ES2::Texture::load_from_memory(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	int w, h, c;

	data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	size.x = static_cast<unsigned int>(w);
	size.y = static_cast<unsigned int>(h);
	type = TextureType::UnsignedByte; //TODO
	format = parse_format(c);


	OPTICK_TAG("id", id);
	OPTICK_TAG("width", size.x);
	OPTICK_TAG("height", size.y);
	OPTICK_TAG("type", to_string(type).c_str());
	OPTICK_TAG("format", to_string(format).c_str());

	if (w <= 0 || h <= 0 || !data || format == TextureFormat::Invalid)
	{
		return false;
	}

	GLenum gl_format = (format == TextureFormat::RGB) ? GL_RGB : GL_RGBA; //TODO
	GLenum gl_type = GL_UNSIGNED_BYTE; //TODO

	glBindTexture(GL_TEXTURE_2D, id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, w, h, 0, gl_format, gl_type, data);

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

TextureFormat ES2::Texture::parse_format(int c) const
{
	OPTICK_EVENT();
	switch (c)
	{
		case 3: return TextureFormat::RGB;
		case 4: return TextureFormat::RGBA;
	}

	return TextureFormat::Invalid;
}

void ES2::Texture::reset()
{
	OPTICK_EVENT();
	STBI_FREE(data);
	data = nullptr;
	length = 0;
	size = { 0, 0, 0 };
	format = TextureFormat::Invalid;
	type = TextureType::Invalid;

	if (glIsTexture(id))
	{
		glDeleteTextures(1, &id);
	}

	id = 0;
}
