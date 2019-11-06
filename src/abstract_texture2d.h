#pragma once
#include "abstract_resource.h"
#include "texture_format.h"
#include "texture_type.h"
#include "abstract_sampler2d.h"
#include "image2d.h"
#include <glm/glm.hpp>
#include <memory>

namespace Rendy
{
	struct AbstractTexture2D : public AbstractResource
	{
		virtual void bind(uint32_t slot) = 0;
		virtual void unbind(uint32_t slot) = 0;
		virtual void set_min_filter(MinFilter filter) = 0;
		virtual void set_mag_filter(MagFilter filter) = 0;
		virtual void set_wrap_mode(WrapMode mode) = 0;
		virtual MinFilter get_min_filter() const = 0;
		virtual MagFilter get_mag_filter() const = 0;
		virtual WrapMode get_wrap_mode() const = 0;
		virtual AbstractSampler2DRef get_sampler() = 0;
		virtual glm::uvec2 get_size() const = 0;
		//virtual TextureFormat get_format() const = 0;
		//virtual TextureType get_type() const = 0;
	};

	using AbstractTexture2DRef = std::shared_ptr<AbstractTexture2D>;
};