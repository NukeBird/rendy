#pragma once
#include <cstdint>

struct AbstractResource
{
	virtual void reload() = 0;
	virtual bool validate() const = 0;
};