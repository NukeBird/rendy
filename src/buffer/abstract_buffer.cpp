#include "abstract_buffer.h"
#include <optick.h>

std::string Rendy::to_string(BufferTarget target)
{
	OPTICK_EVENT();

	switch(target)
	{
		case BufferTarget::VBO:
			return "VBO";
		case BufferTarget::IBO:
			return "IBO";
	}

	return "Invalid";
}
