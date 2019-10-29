#pragma once
#include "abstract_texture_cube.h"
#include "abstract_command.h"

class BindTextureCube: public AbstractCommand
{
public:
	BindTextureCube(AbstractTextureCubeRef texture, uint32_t slot);
	virtual void execute() override;
private:
	AbstractTextureCubeRef texture;
	uint32_t slot;
};

using BindTextureCubeRef = std::shared_ptr<BindTextureCube>;