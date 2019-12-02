#pragma once
#include "../ogl.hpp"
#include "abstract_buffer.h"

namespace Rendy
{
	struct AbstractBufferFactory
	{
		virtual AbstractBufferRef make(BufferTarget target, uint32_t size,
			const void* ptr) = 0;
	};
};