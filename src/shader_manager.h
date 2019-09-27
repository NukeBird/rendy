#pragma once
#include "abstract_shader.h"
#include "singleton.h"
#include <vector>
#include <memory>

using AbstractShaderRef = std::shared_ptr<AbstractShader>;

class ShaderManager: public Singleton<ShaderManager>
{
public:
	//TODO: make from memory?
	AbstractShaderRef make(const std::string& vtx, const std::string& frg);
	void reload();
	void flush();
private:
	std::vector<AbstractShaderRef> shader_list;
};