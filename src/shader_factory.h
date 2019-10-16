#pragma once
#include "abstract_shader.h"
#include "singleton.h"
#include <vector>
#include <memory>

class ShaderFactory: public Singleton<ShaderFactory> //TODO: should store and return only ONE generic shader
{
public:
	//TODO: make from memory?
	AbstractShaderRef make(const std::string& vtx, const std::string& frg);
	AbstractShaderRef get_generic_shader();
private:
	AbstractShaderRef generic_shader;
};