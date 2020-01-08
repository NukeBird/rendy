#include "bind_texture_cube.h"
#include <optick.h>

Rendy::BindTextureCube::BindTextureCube(AbstractTextureCubeRef texture, uint32_t slot)
{
	OPTICK_EVENT();

	this->texture = texture;
	this->slot = slot;
}

void Rendy::BindTextureCube::execute()
{
	OPTICK_EVENT();

	if (texture)
	{
		texture->bind(slot);
	}
}
