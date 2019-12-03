#pragma once
#include "../../ogl.hpp"
#include "abstract_sampler2d.h"
#include <memory>
#include <string>

namespace Rendy
{
	class Sampler2DFactory final
	{
	public:
		Sampler2DFactory(OGL version);
		AbstractSampler2DRef make();
	private:
		OGL version;
	};

	using Sampler2DFactoryRef = std::shared_ptr<Sampler2DFactory>;
};