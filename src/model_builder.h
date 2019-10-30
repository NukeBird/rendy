#pragma once
#include "model.h"

namespace Rendy
{
	class ModelBuilder
	{
	public:
		static ModelRef build(const char* filename);
	};
};