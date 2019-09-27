#pragma once
#include "abstract_resource.h"
#include "texture_format.h"
#include "texture_type.h"
#include <glm/vec3.hpp>

struct AbstractTexture: public AbstractResource
{
	virtual ~AbstractTexture() = default; //TODO: = 0?
	virtual void bind(uint32_t slot) = 0;
	virtual void unbind(uint32_t slot) = 0;
	virtual glm::uvec3 get_size() const = 0;
	virtual TextureFormat get_format() const = 0;
	virtual TextureType get_type() const = 0;
};