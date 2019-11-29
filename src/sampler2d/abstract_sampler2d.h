#pragma once
#include "../abstract_resource.h"
#include "min_filter.h"
#include "mag_filter.h"
#include "wrap_mode.h"
#include <memory>

namespace Rendy
{
	struct AbstractSampler2D: public AbstractResource
	{
		virtual void bind(uint32_t slot) = 0;
		virtual void unbind(uint32_t slot) = 0;
		virtual void set_min_filter(MinFilter filter) = 0;
		virtual void set_mag_filter(MagFilter filter) = 0;
		virtual void set_wrap_mode(WrapMode mode) = 0;
		virtual MinFilter get_min_filter() const = 0;
		virtual MagFilter get_mag_filter() const = 0;
		virtual WrapMode get_wrap_mode() const = 0;
	};

	using AbstractSampler2DRef = std::shared_ptr<AbstractSampler2D>;
};