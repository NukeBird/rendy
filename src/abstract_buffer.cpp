#include "abstract_buffer.h"

std::string to_string(BufferTarget target)
{
	switch(target)
	{
		case BufferTarget::VBO:
			return "VBO";
		case BufferTarget::IBO:
			return "IBO";
	}

	return "Invalid";
}
