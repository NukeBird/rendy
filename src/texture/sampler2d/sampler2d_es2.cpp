#include "sampler2d_es2.h"
#include <optick.h>

void Rendy::ES2::Sampler2D::reload()
{
	//do nothing
	OPTICK_EVENT();
}

bool Rendy::ES2::Sampler2D::validate() const
{
	OPTICK_EVENT();
	return true; //?
}

void Rendy::ES2::Sampler2D::bind(uint32_t slot)
{
	OPTICK_EVENT();

	auto gl_wrap_mode = parse_wrap_mode(wrap_mode);
	auto gl_min_filter = parse_min_filter(min_filter);
	auto gl_mag_filter = parse_mag_filter(mag_filter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter);
}

void Rendy::ES2::Sampler2D::unbind(uint32_t slot)
{
	//do nothing
	OPTICK_EVENT();
}

void Rendy::ES2::Sampler2D::set_min_filter(MinFilter filter)
{
	OPTICK_EVENT();
	min_filter = filter;
}

void Rendy::ES2::Sampler2D::set_mag_filter(MagFilter filter)
{
	OPTICK_EVENT();
	mag_filter = filter;
}

void Rendy::ES2::Sampler2D::set_wrap_mode(WrapMode mode)
{
	OPTICK_EVENT();
	wrap_mode = mode;
}

Rendy::MinFilter Rendy::ES2::Sampler2D::get_min_filter() const
{
	OPTICK_EVENT();
	return min_filter;
}

Rendy::MagFilter Rendy::ES2::Sampler2D::get_mag_filter() const
{
	OPTICK_EVENT();
	return mag_filter;
}

Rendy::WrapMode Rendy::ES2::Sampler2D::get_wrap_mode() const
{
	OPTICK_EVENT();
	return wrap_mode;
}

GLenum Rendy::ES2::Sampler2D::parse_min_filter(MinFilter filter) const
{
	OPTICK_EVENT();

	switch (filter)
	{
		case Rendy::MinFilter::Nearest:
		{
			return GL_NEAREST;
		}
		case Rendy::MinFilter::Linear:
		{
			return GL_LINEAR;
		}
		case Rendy::MinFilter::NearestMipmapNearest:
		{
			return GL_NEAREST_MIPMAP_NEAREST;
		}
		case Rendy::MinFilter::LinearMipmapNearest:
		{
			return GL_LINEAR_MIPMAP_NEAREST;
		}
		case Rendy::MinFilter::NearestMipmapLinear:
		{
			return GL_NEAREST_MIPMAP_LINEAR;
		}
		case Rendy::MinFilter::LinearMipmapLinear:
		{
			return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	return GL_INVALID_ENUM;
}

GLenum Rendy::ES2::Sampler2D::parse_mag_filter(MagFilter filter) const
{
	OPTICK_EVENT();

	switch (filter)
	{
		case Rendy::MagFilter::Nearest:
		{
			return GL_NEAREST;
		}
		case Rendy::MagFilter::Linear:
		{
			return GL_LINEAR;
		}
	}

	return GL_INVALID_ENUM;
}

GLenum Rendy::ES2::Sampler2D::parse_wrap_mode(WrapMode mode) const
{
	OPTICK_EVENT();

	switch (mode)
	{
		case Rendy::WrapMode::ClampToEdge:
		{
			return GL_CLAMP_TO_EDGE;
		}
		case Rendy::WrapMode::MirroredRepeat:
		{
			return GL_MIRRORED_REPEAT;
		}
		case Rendy::WrapMode::Repeat:
		{
			return GL_REPEAT;
		}
	}

	return GL_INVALID_ENUM;
}
