#pragma once
#include <initializer_list>
#include <cstdint>
#include <vector>
#include <memory>
#include "../shader/shader_data_type.h"
#include <string>

namespace Rendy
{
	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name);

		uint32_t get_component_count() const;
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::vector<BufferElement>& elements);

		uint32_t get_stride() const;
		const std::vector<BufferElement>& get_elements() const;

		std::vector<BufferElement>::iterator begin();
		std::vector<BufferElement>::iterator end();
		std::vector<BufferElement>::const_iterator begin() const;
		std::vector<BufferElement>::const_iterator end() const;
	private:
		void calculate();

		std::vector<BufferElement> elements;
		uint32_t stride = 0;
	};

	using BufferLayoutRef = std::shared_ptr<BufferLayout>;
};