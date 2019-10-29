#pragma once
#include <memory>

struct AbstractCommand
{
	virtual void execute() = 0;
};

using CommandCallRef = std::shared_ptr<AbstractCommand>;