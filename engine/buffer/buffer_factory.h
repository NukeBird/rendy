#pragma once
#include "../ogl.hpp"
#include "abstract_buffer.h"
#include <memory>

namespace Rendy
{
	class BufferFactory final
	{
	public:
		BufferFactory(OGL version);
		AbstractBufferRef make(BufferTarget target, uint32_t size,
			const void* ptr);
	private:
		OGL version;
	};

	using BufferFactoryRef = std::shared_ptr<BufferFactory>;
};