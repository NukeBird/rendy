#include "draw.h"

Rendy::Draw::Draw(AbstractVertexArrayRef vao)
{
	this->vao = vao;
}

void Rendy::Draw::execute()
{
	if (vao)
	{
		vao->draw();
	}
}
