#pragma once
#include "abstract_buffer.h"
#include <vector>
#include "../common.h"

namespace Rendy
{
	namespace ES2
	{
		class Buffer: public AbstractBuffer
		{
		public:
			Buffer(BufferTarget target, uint32_t size, const void* ptr);
			~Buffer();
			virtual void reload() override;
			virtual bool validate() const override;
			virtual BufferTarget get_target() const override;
			virtual uint32_t get_size() const override;
			virtual void set_data(uint32_t size, const void* ptr) override;
			virtual void bind() override;
			virtual void bind(uint32_t index) override;
			virtual void unbind() override;
		protected:
			void generate_buffer();
			GLenum get_gl_target() const;

			std::vector<uint8_t> data;
			BufferTarget target = BufferTarget::Invalid;
			uint32_t id;
		};
	};
};