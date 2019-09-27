#include "shader_data_type.h"

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
