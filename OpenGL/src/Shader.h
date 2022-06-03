#ifndef MY_SHADER_H
#define MY_SHADER_H

#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;

public:
	Shader(const std::string& VSpath, const std::string& FSpath);
	~Shader();

	void bind();
	void unbind();
};

#endif
