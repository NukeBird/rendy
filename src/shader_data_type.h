#pragma once
#include <cstdint>

enum class ShaderDataType
{
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

uint32_t get_shader_data_type_size(ShaderDataType type);