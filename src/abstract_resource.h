#pragma once
#include <cstdint>

struct AbstractResource
{
	virtual ~AbstractResource() = default; //TODO: = 0?
	virtual void reload() = 0;
	virtual bool validate() const = 0;
};