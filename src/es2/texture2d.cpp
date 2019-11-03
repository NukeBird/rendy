#include "texture2d.h"
#include "../common.h"
#include <mutex> //call_once
#include <optick.h>

Rendy::ES2::Texture2D::Texture2D(Image2DRef image)
{
	OPTICK_EVENT();

	this->image = image;

	glGenTextures(1, &id);

	if (!load_from_image())
	{
		reset();
	}
}

Rendy::ES2::Texture2D::~Texture2D()
{
	OPTICK_EVENT();
	reset();
}

void Rendy::ES2::Texture2D::reload()
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

bool Rendy::ES2::Texture2D::validate() const
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

void Rendy::ES2::Texture2D::bind(uint32_t slot)
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

void Rendy::ES2::Texture2D::unbind(uint32_t slot)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("slot", slot);

	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::uvec2 Rendy::ES2::Texture2D::get_size() const
{
	OPTICK_EVENT();
	return image->get_size();
}

/*Rendy::TextureFormat Rendy::ES2::Texture2D::get_format() const
{
	OPTICK_EVENT();
	return format;
}*/

/*Rendy::TextureType Rendy::ES2::Texture2D::get_type() const
{
	OPTICK_EVENT();
	return type;
}*/

bool Rendy::ES2::Texture2D::load_from_image()
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

	auto image_type = image->get_type();

	/*if (image_type != TextureType::UnsignedByte)
	{
		return false;
	}*/

	GLenum gl_format = (format == TextureFormat::RGB) ? GL_RGB : GL_RGBA; //TODO
	GLenum gl_type = (image_type == TextureType::UnsignedByte) ? GL_UNSIGNED_BYTE : GL_HALF_FLOAT; //TODO

	bind(0);

	OPTICK_PUSH("glPixelStorei");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //TODO
	OPTICK_POP();

	OPTICK_PUSH("glTexImage2D");
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, 
		image->get_width(), image->get_height(), 
		0, gl_format, gl_type, image->get_data());
	OPTICK_POP();

	OPTICK_PUSH("glGenerateMipmap");
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	glGenerateMipmap(GL_TEXTURE_2D);
	OPTICK_POP();

	unbind(0);

	return true;
}

Rendy::TextureFormat Rendy::ES2::Texture2D::parse_format(int c) const
{
	OPTICK_EVENT();
	switch (c)
	{
		case 3: return TextureFormat::RGB;
		case 4: return TextureFormat::RGBA;
	}

	return TextureFormat::Invalid;
}

void Rendy::ES2::Texture2D::reset()
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
