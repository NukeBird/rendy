#pragma once
#include "scene.h"

struct AbstractEngine
{
	virtual ~AbstractEngine() = default; //TODO: = 0?
	virtual void render(const SceneRef& scene) = 0;
};