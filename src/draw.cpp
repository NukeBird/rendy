#include "draw.h"

Draw::Draw(AbstractVertexArrayRef vao)
{
	this->vao = vao;
}

void Draw::execute()
{
	if (vao)
	{
		vao->draw();
	}
}
