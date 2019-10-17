#pragma once
#include "scene.h"
#include <glm/glm.hpp>

struct AbstractEngine
{
	virtual ~AbstractEngine() = default; //TODO: = 0?
	virtual void render(const SceneRef& scene, const glm::mat4& model, 
		const glm::mat4& view, const glm::mat4& proj) = 0;
};