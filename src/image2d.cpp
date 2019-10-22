#include "image2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image2D::Image2D(const char* memory, uint32_t length)
{
	load_image(memory, length);
}

Image2D::~Image2D()
{
	STBI_FREE(data_ptr);
	data_ptr = nullptr;
	size = glm::uvec2{ 0, 0 };
	channel_count = 0;
}

void* Image2D::get_data()
{
	return data_ptr;
}

glm::uvec2 Image2D::get_size() const
{
	return size;
}

uint32_t Image2D::get_width() const
{
	return get_size().x;
}

uint32_t Image2D::get_height() const
{
	return get_size().y;
}

uint32_t Image2D::get_channel_count() const
{
	return channel_count;
}

void Image2D::reload()
{
	//do nothing?
}

bool Image2D::validate() const
{
	if (data_ptr == nullptr)
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

void Image2D::load_image(const char* memory, uint32_t length)
{
	int w, h, c;

	data_ptr = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	size.x = static_cast<uint32_t>(w);
	size.y = static_cast<uint32_t>(h);
	channel_count = static_cast<uint32_t>(c);
}
