#pragma once
#include <memory>

struct AbstractCommandCall
{
	virtual void execute() = 0;
};

using CommandCallRef = std::shared_ptr<AbstractCommandCall>;