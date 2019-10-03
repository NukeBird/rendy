#pragma once
#include "abstract_shader.h"
#include "singleton.h"
#include <vector>
#include <memory>

class ShaderManager: public Singleton<ShaderManager> //TODO: should store and return only ONE generic shader
{
public:
	//TODO: make from memory?
	AbstractShaderRef make(const std::string& vtx, const std::string& frg);
	AbstractShaderRef get_generic_shader();
	void reload();
	void flush();
private:
	std::vector<AbstractShaderRef> shader_list;
	AbstractShaderRef generic_shader;
};