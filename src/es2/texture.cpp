#include "texture.h"
#include <GL/glew.h> //TODO
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ES2::Texture::Texture(const char* memory, uint32_t length)
{
	glGenTextures(1, &id);

	this->length = length;

	if (!load_from_memory(memory, length))
	{
		reset();
	}
}

ES2::Texture::~Texture()
{
	reset();
}

void ES2::Texture::reload()
{
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

	if (size.x == 0 || size.y == 0 || size.z == 0)
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
	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void ES2::Texture::unbind(uint32_t slot)
{
	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::uvec3 ES2::Texture::get_size() const
{
	return size;
}

TextureFormat ES2::Texture::get_format() const
{
	return format;
}

TextureType ES2::Texture::get_type() const
{
	return type;
}

bool ES2::Texture::load_from_memory(const char* memory, uint32_t length)
{
	int w, h, c;

	data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	size.x = static_cast<unsigned int>(w);
	size.y = static_cast<unsigned int>(h);
	type = TextureType::UnsignedByte; //TODO
	format = parse_format(c);

	if (w <= 0 || h <= 0 || !data || format == TextureFormat::Invalid)
	{
		return false;
	}

	GLenum gl_format = (format == TextureFormat::RGB) ? GL_RGB : GL_RGBA; //TODO
	GLenum gl_type = GL_UNSIGNED_BYTE; //TODO

	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, w, h, 0, gl_format, gl_type, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

TextureFormat ES2::Texture::parse_format(int c) const
{
	switch (c)
	{
		case 3: return TextureFormat::RGB;
		case 4: return TextureFormat::RGBA;
	}

	return TextureFormat::Invalid;
}

void ES2::Texture::reset()
{
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
