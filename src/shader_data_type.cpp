#include "shader_data_type.h"

std::string to_string(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:
			return "Float";
		case ShaderDataType::Float2:
			return "Float2";
		case ShaderDataType::Float3:
			return "Float3";
		case ShaderDataType::Float4:
			return "Float4";
		case ShaderDataType::Mat3:
			return "Mat3";
		case ShaderDataType::Mat4:
			return "Mat4";
		case ShaderDataType::Int:
			return "Int";
		case ShaderDataType::Int2:
			return "Int2";
		case ShaderDataType::Int3:
			return "Int3";
		case ShaderDataType::Int4:
			return "Int4";
		case ShaderDataType::Bool:
			return "Bool";
	}

	return "WTF";
}

uint32_t get_shader_data_type_size(ShaderDataType type)
{
	uint32_t size = 0;

	switch (type)
	{
		case ShaderDataType::Float:
		{
			size = 4;
			break;
		}

		case ShaderDataType::Float2:
		{
			size = 4 * 2;
			break;
		}

		case ShaderDataType::Float3:
		{
			size = 4 * 3;
			break;
		}

		case ShaderDataType::Float4:
		{
			size = 4 * 4;
			break;
		}

		case ShaderDataType::Mat3:
		{
			size = 4 * 3 * 3;
			break;
		}

		case ShaderDataType::Mat4:
		{
			size = 4 * 4 * 4;
			break;
		}

		case ShaderDataType::Int:
		{
			size = 4;
			break;
		}

		case ShaderDataType::Int2:
		{
			size = 4 * 2;
			break;
		}

		case ShaderDataType::Int3:
		{
			size = 4 * 3;
			break;
		}

		case ShaderDataType::Int4:
		{
			size = 4 * 4;
			break;
		}

		case ShaderDataType::Bool:
		{
			size = 1;
			break;
		}
	}

	return size;
}
