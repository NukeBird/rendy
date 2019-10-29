#pragma once
#include <memory>

struct AbstractCommand
{
	virtual void execute() = 0;
};

using CommandRef = std::shared_ptr<AbstractCommand>;