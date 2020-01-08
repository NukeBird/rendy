#pragma once
#include <cstdint>

namespace Rendy
{
	struct AbstractResource
	{
		virtual void reload() = 0;
		virtual bool validate() const = 0;
	};
};