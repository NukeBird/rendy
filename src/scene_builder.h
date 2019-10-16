#pragma once
#include "scene.h"

struct SceneBuilder
{
	static SceneRef build(const char* filename); 
	//TODO: build from memory
};