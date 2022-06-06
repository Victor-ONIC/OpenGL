#ifndef MY_SHADER_H
#define MY_SHADER_H

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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

	void bind() const;
	void unbind() const;

	void uniform_1i(const std::string& name, int value) const;
	void uniform_mat4(const std::string& name, const glm::mat4& matrice) const ;
};

#endif
