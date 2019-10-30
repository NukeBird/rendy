#pragma once
#include "image2d.h"
#include <memory>

namespace Rendy
{
	struct ImageSet
	{
		Image2DRef color;
		Image2DRef normal;
		Image2DRef metallic_roughness;
	};

	using ImageSetRef = std::shared_ptr<ImageSet>;
};