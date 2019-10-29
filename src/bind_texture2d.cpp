#include "bind_texture2d.h"

BindTexture2D::BindTexture2D(AbstractTexture2DRef texture, uint32_t slot)
{
	this->texture = texture;
	this->slot = slot;
}

void BindTexture2D::execute()
{
	if (texture)
	{
		texture->bind(slot); //TODO: possibly invalid slot?
	}
}
