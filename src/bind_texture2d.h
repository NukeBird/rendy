#pragma once
#include "abstract_texture2d.h"
#include "abstract_command.h"

class BindTexture2D: public AbstractCommand
{
public:
	BindTexture2D(AbstractTexture2DRef texture, uint32_t slot);
	virtual void execute() override;
private:
	AbstractTexture2DRef texture;
	uint32_t slot;
};

using BindTexture2DRef = std::shared_ptr<BindTexture2D>;