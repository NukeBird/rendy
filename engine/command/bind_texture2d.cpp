#include "bind_texture2d.h"
#include <optick.h>

Rendy::BindTexture2D::BindTexture2D(AbstractTexture2DRef texture, uint32_t slot)
{
	OPTICK_EVENT();

	this->texture = texture;
	this->slot = slot;
}

void Rendy::BindTexture2D::execute()
{
	OPTICK_EVENT();

	if (texture)
	{
		texture->bind(slot); //TODO: possibly invalid slot?
	}
}
