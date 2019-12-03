#include "image2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <optick.h>
#include "../common.h"
#include "../util/log.h"

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
	OPTICK_EVENT();
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

bool Rendy::Image2D::uses_transparency() const
{
	OPTICK_EVENT();
	return has_useful_alpha;
}

void Rendy::Image2D::load_image(const char* memory, uint32_t length)
{
	OPTICK_EVENT();

	Log::info("Loading image...");

	int w, h, c;

	data_ptr = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	if (data_ptr)
	{
		type = TextureType::UnsignedByte;
		size.x = static_cast<uint32_t>(w);
		size.y = static_cast<uint32_t>(h);
		channel_count = static_cast<uint32_t>(c);
		Log::info("Image loaded using stbi_image");
	}
	else
	{
		Log::warn("Can't load image using stbi_image, let's try gli...");
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
			Log::error("Can't load image using gli");
			gli_tex.clear();
			return;
		}

		Log::info("Image loaded using gli");

		size.x = static_cast<uint32_t>(gli_tex.extent(gli_tex.base_level()).x);
		size.y = static_cast<uint32_t>(gli_tex.extent(gli_tex.base_level()).y);
		channel_count = 4; //TODO
		type = TextureType::HalfFloat; //TODO
	}

	analyze_alpha_channel();
	Log::info
	(
		"Image parameters:\n"
		"Size: {0}x{1}\n"
		"Channel count: {2}",
		size.x, size.y, channel_count
	);
	OPTICK_TAG("width", size.x);
	OPTICK_TAG("height", size.y);
	OPTICK_TAG("channel count", channel_count);
}

void Rendy::Image2D::analyze_alpha_channel()
{
	OPTICK_EVENT();

	if (channel_count == 4 && data_ptr)
	{
		for (uint32_t i = 0; i < size.x * size.y; ++i)
		{
			if (static_cast<glm::u8vec4*>(data_ptr)[i].a < 255.0f)
			{
				has_useful_alpha = true;
				break;
			}
		}
	}
	else
	{
		has_useful_alpha = false;
	}

	Log::info("Image uses alpha: {0}", has_useful_alpha);
}
