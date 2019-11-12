#include "image2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <optick.h>
#include "common.h"

Rendy::Image2D::Image2D(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	OPTICK_TAG("size", length);

	load_image(memory, length);
}

Rendy::Image2D::~Image2D()
{
	OPTICK_EVENT();

	STBI_FREE(data_ptr);
	data_ptr = nullptr;
	size = glm::uvec2{ 0, 0 };
	channel_count = 0;
}

void* Rendy::Image2D::get_data()
{
	OPTICK_EVENT();

	if (gli_tex.empty())
	{
		return data_ptr;
	}

	return gli_tex.data();
}

glm::uvec2 Rendy::Image2D::get_size() const
{
	OPTICK_EVENT();
	return size;
}

uint32_t Rendy::Image2D::get_width() const
{
	OPTICK_EVENT();
	return get_size().x;
}

uint32_t Rendy::Image2D::get_height() const
{
	OPTICK_EVENT();
	return get_size().y;
}

Rendy::TextureType Rendy::Image2D::get_type() const
{
	return type;
}

uint32_t Rendy::Image2D::get_channel_count() const
{
	OPTICK_EVENT();
	return channel_count;
}

void Rendy::Image2D::reload()
{
	OPTICK_EVENT();
	//do nothing?
}

bool Rendy::Image2D::validate() const
{
	OPTICK_EVENT();
	if ((data_ptr == nullptr) && gli_tex.empty())
	{
		return false;
	}

	if (get_width() == 0 || get_height() == 0)
	{
		return false;
	}

	if (get_channel_count() == 0)
	{
		return false;
	}

	return true;
}

void Rendy::Image2D::load_image(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	int w, h, c;

	data_ptr = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	if (data_ptr)
	{
		type = TextureType::UnsignedByte;
		size.x = static_cast<uint32_t>(w);
		size.y = static_cast<uint32_t>(h);
		channel_count = static_cast<uint32_t>(c);
	}
	else
	{
		bool fail = false;

		gli_tex = gli::load(memory, length);
		gli::gl GL(gli::gl::PROFILE_ES30);
		gli::gl::format const format = GL.translate(gli_tex.format(), gli_tex.swizzles());

		if (gli_tex.empty() || gli_tex.target() != gli::target::TARGET_2D)
		{
			fail = true;
		}
		else
		{ 
			if (gli_tex.format() !=
				gli::texture::format_type::FORMAT_RGBA16_SFLOAT_PACK16) //TODO
			{
				fail = true;
			}
		}

		if (fail)
		{
			gli_tex.clear();
			return;
		}

		size.x = static_cast<uint32_t>(gli_tex.extent(gli_tex.base_level()).x);
		size.y = static_cast<uint32_t>(gli_tex.extent(gli_tex.base_level()).y);
		channel_count = 4; //TODO
		type = TextureType::HalfFloat; //TODO

	}

	OPTICK_TAG("width", size.x);
	OPTICK_TAG("height", size.y);
	OPTICK_TAG("channel count", channel_count);
}
