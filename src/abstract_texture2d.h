#pragma once
#include "abstract_resource.h"
#include "texture_format.h"
#include "texture_type.h"
#include "image2d.h"
#include <glm/glm.hpp>
#include <memory>

namespace Rendy
{
	struct AbstractTexture2D : public AbstractResource
	{
		virtual void bind(uint32_t slot) = 0;
		virtual void unbind(uint32_t slot) = 0;
		virtual glm::uvec2 get_size() const = 0;
		//virtual TextureFormat get_format() const = 0;
		//virtual TextureType get_type() const = 0;
	};

	using AbstractTexture2DRef = std::shared_ptr<AbstractTexture2D>;
};