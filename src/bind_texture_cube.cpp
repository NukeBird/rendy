#include "bind_texture_cube.h"

BindTextureCube::BindTextureCube(AbstractTextureCubeRef texture, uint32_t slot)
{
	this->texture = texture;
	this->slot = slot;
}

void BindTextureCube::execute()
{
	if (texture)
	{
		texture->bind(slot);
	}
}
