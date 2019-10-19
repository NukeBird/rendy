#pragma once
#include <cstdint>
#include <string>

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

std::string to_string(ShaderDataType type);

uint32_t get_shader_data_type_size(ShaderDataType type);