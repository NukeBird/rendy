#pragma once
#include <memory>
#include <vector>

namespace Rendy
{
	struct AbstractCommand
	{
		virtual void execute() = 0;
	};

	using CommandRef = std::shared_ptr<AbstractCommand>;
	using CommandList = std::vector<CommandRef>;
};