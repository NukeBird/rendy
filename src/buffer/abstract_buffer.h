#pragma once
#include "../abstract_resource.h"
#include "../common.h"
#include <memory>
#include <string>
#include <vector>

namespace Rendy
{
	enum class BufferTarget
	{
		VertexArray,
		ElementArray,
		CBO,
		SBO,
		Invalid
	};

	std::string to_string(BufferTarget target);

	struct AbstractBuffer: public AbstractResource
	{
		AbstractBuffer(BufferTarget target, uint32_t size, const void* ptr);
		virtual ~AbstractBuffer();
		virtual BufferTarget get_target() const;
		virtual uint32_t get_size() const;
		virtual void set_data(uint32_t size, const void* ptr);
		virtual void reload() override;
		virtual bool validate() const override;
		virtual void bind();
		virtual void bind(uint32_t index) = 0;
		virtual void unbind();
	protected:
		void generate_buffer();
		GLenum get_gl_target() const;

		std::vector<uint8_t> data;
		BufferTarget target = BufferTarget::Invalid;
		uint32_t id;
	};

	using AbstractBufferRef = std::shared_ptr<AbstractBuffer>;
};