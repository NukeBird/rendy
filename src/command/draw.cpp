#include "draw.h"
#include <optick.h>

Rendy::Draw::Draw(AbstractVertexArrayRef vao)
{
	OPTICK_EVENT();
	this->vao = vao;
}

void Rendy::Draw::execute()
{
	OPTICK_EVENT();

	if (vao)
	{
		vao->draw();
	}
}
