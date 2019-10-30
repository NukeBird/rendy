#pragma once
#include "model.h"

namespace Rendy
{
	class ModelFactory
	{
	public:
		static ModelRef build(const char* filename);
	};
};