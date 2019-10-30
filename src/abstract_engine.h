#pragma once
#include "abstract_buffer.h"
#include "abstract_drawable.h"
#include "abstract_command.h"
#include "batch.h"
#include <vector>

namespace Rendy
{
	using CommandList = std::vector<CommandRef>;

	struct AbstractEngine
	{
		virtual void push(BatchList batches) = 0;
		virtual void push(CommandList commands) = 0;
		virtual void flush() = 0;
		virtual void reload() = 0;

		virtual AbstractBufferRef make_vbo(uint32_t size, const void* ptr) = 0;
	};
};