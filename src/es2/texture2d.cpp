#include "texture2d.h"
#include "../common.h"
#include <mutex> //call_once
#include <optick.h>

Rendy::ES2::Texture2D::Texture2D(Image2DRef image)
{
	OPTICK_EVENT();

	this->image = image;
	this->sampler = std::make_shared<Sampler2D>();
	sampler->set_min_filter(MinFilter::LinearMipmapLinear);
	sampler->set_mag_filter(MagFilter::Linear);

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

	sampler->reload();
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

	if (!sampler)
	{
		return false;
	}

	if (!sampler->validate())
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

	sampler->bind(slot);
}

void Rendy::ES2::Texture2D::unbind(uint32_t slot)
{
	OPTICK_EVENT();
	OPTICK_TAG("id", id);
	OPTICK_TAG("slot", slot);

	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);

	sampler->unbind(slot);
}

void Rendy::ES2::Texture2D::set_min_filter(MinFilter filter)
{
	OPTICK_EVENT();
	sampler->set_min_filter(filter);
}

void Rendy::ES2::Texture2D::set_mag_filter(MagFilter filter)
{
	OPTICK_EVENT();
	sampler->set_mag_filter(filter);
}

void Rendy::ES2::Texture2D::set_wrap_mode(WrapMode mode)
{
	OPTICK_EVENT();
	sampler->set_wrap_mode(mode);
}

bool Rendy::ES2::Texture2D::uses_transparency() const
{
	OPTICK_EVENT();
	return image->uses_transparency();
}

Rendy::MinFilter Rendy::ES2::Texture2D::get_min_filter() const
{
	OPTICK_EVENT();
	return sampler->get_min_filter();
}

Rendy::MagFilter Rendy::ES2::Texture2D::get_mag_filter() const
{
	OPTICK_EVENT();
	return sampler->get_mag_filter();
}

Rendy::WrapMode Rendy::ES2::Texture2D::get_wrap_mode() const
{
	OPTICK_EVENT();
	return sampler->get_wrap_mode();
}

Rendy::AbstractSampler2DRef Rendy::ES2::Texture2D::get_sampler()
{
	OPTICK_EVENT();
	return sampler;
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
	//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
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
