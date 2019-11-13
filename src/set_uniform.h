#pragma once
#include "abstract_command.h"
#include "abstract_shader_variant.h"

namespace Rendy
{
	struct AbstractSetUniform : public AbstractCommand {}; //TODO: move?

	using AbstractSetUniformRef = std::shared_ptr<AbstractSetUniform>;

	template <class T>
	class SetUniform: public AbstractSetUniform
	{
	public:
		SetUniform(ShaderVariantRef shader_variant, const std::string& uniform_name,
			const T& value);
		virtual void execute() override;
	private:
		ShaderVariantRef shader_variant;
		std::string uniform_name;
		T value;
	};

	template<class T>
	inline SetUniform<T>::SetUniform(ShaderVariantRef shader_variant, const std::string& uniform_name, const T& value)
	{
		this->shader_variant = shader_variant;
		this->uniform_name = uniform_name;
		this->value = value;
	}

	template<class T>
	inline void SetUniform<T>::execute()
	{
		if (shader_variant)
		{
			shader_variant->set_uniform(uniform_name, value);
		}
	}

	using SetUniformVec3 = SetUniform<glm::vec3>;
	using SetUniformMat4 = SetUniform<glm::mat4>;
	using SetUniformMat3 = SetUniform<glm::mat3>;
	using SetUniformFloat = SetUniform<float>;
	using SetUniformInt = SetUniform<int>;

	using SetUniformVec3Ref = std::shared_ptr<SetUniformVec3>;
	using SetUniformMat4Ref = std::shared_ptr<SetUniformMat4>;
	using SetUniformMat3Ref = std::shared_ptr<SetUniformMat3>;
	using SetUniformFloatRef = std::shared_ptr<SetUniformFloat>;
	using SetUniformIntRef = std::shared_ptr<SetUniformInt>;

	using Vec3Array = std::vector<glm::vec3>;
	using Mat4Array = std::vector<glm::mat4>;
	using Mat3Array = std::vector<glm::mat3>;
	using FloatArray = std::vector<float>;
	using IntArray = std::vector<int>;

	using SetUniformVec3Array = SetUniform<Vec3Array>;
	using SetUniformMat4Array = SetUniform<Mat4Array>;
	using SetUniformMat3Array = SetUniform<Mat3Array>;
	using SetUniformFloatArray = SetUniform<FloatArray>;
	using SetUniformIntArray = SetUniform<IntArray>;

	using SetUniformVec3ArrayRef = std::shared_ptr<SetUniformVec3Array>;
	using SetUniformMat4ArrayRef = std::shared_ptr<SetUniformMat4Array>;
	using SetUniformMat3ArrayRef = std::shared_ptr<SetUniformMat3Array>;
	using SetUniformFloatArrayRef = std::shared_ptr<SetUniformFloatArray>;
	using SetUniformIntArrayRef = std::shared_ptr<SetUniformIntArray>;
};