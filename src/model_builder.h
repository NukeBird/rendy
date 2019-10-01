#pragma once
#include "model.h"

class ModelBuilder
{
public:
	static ModelRef build(const char* filename);
};