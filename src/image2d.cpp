#include "image2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <optick.h>

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
	return data_ptr;
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

void Rendy::Image2D::load_image(const char* memory, uint32_t length)
{
	OPTICK_EVENT();
	int w, h, c;

	data_ptr = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memory),
		static_cast<int>(length), &w, &h, &c, 0);

	size.x = static_cast<uint32_t>(w);
	size.y = static_cast<uint32_t>(h);
	channel_count = static_cast<uint32_t>(c);

	OPTICK_TAG("width", size.x);
	OPTICK_TAG("height", size.y);
	OPTICK_TAG("channel count", channel_count);
}
