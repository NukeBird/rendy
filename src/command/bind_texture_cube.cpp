#include "bind_texture_cube.h"

Rendy::BindTextureCube::BindTextureCube(AbstractTextureCubeRef texture, uint32_t slot)
{
	this->texture = texture;
	this->slot = slot;
}

void Rendy::BindTextureCube::execute()
{
	if (texture)
	{
		texture->bind(slot);
	}
}
