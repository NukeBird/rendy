#include "texture.h"
#include <GL/glew.h> //TODO
#include <mutex> //call_once
#include <optick.h>

ES2::Texture::Texture(Image2DRef image)
{
	OPTICK_EVENT();

	this->image = image;

	glGenTextures(1, &id);

	if (!load_from_image())
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
		if (!load_from_image())
		{
			reset();
		}
	}
}

bool ES2::Texture::validate() const
{
	OPTICK_EVENT();
	if (!image)
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

glm::uvec2 ES2::Texture::get_size() const
{
	OPTICK_EVENT();
	return image->get_size();
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

bool ES2::Texture::load_from_image()
{
	OPTICK_EVENT();
	
	type = TextureType::UnsignedByte; //TODO?
	format = parse_format(image->get_channel_count());


	OPTICK_TAG("id", id);
	OPTICK_TAG("type", to_string(type).c_str());
	OPTICK_TAG("format", to_string(format).c_str());

	if (format == TextureFormat::Invalid || !image->validate())
	{
		return false;
	}

	GLenum gl_format = (format == TextureFormat::RGB) ? GL_RGB : GL_RGBA; //TODO
	GLenum gl_type = GL_UNSIGNED_BYTE; //TODO

	glBindTexture(GL_TEXTURE_2D, id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, 
		image->get_width(), image->get_height(), 
		0, gl_format, gl_type, image->get_data());

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
	format = TextureFormat::Invalid;
	type = TextureType::Invalid;

	if (glIsTexture(id))
	{
		glDeleteTextures(1, &id);
	}

	id = 0;
}
