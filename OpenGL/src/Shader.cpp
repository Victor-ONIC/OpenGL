#include "Shader.h"

Shader::Shader(const std::string& VSpath, const std::string& FSpath)
{
	// Get shaders code
	std::ifstream VSfile(VSpath);
	if (!VSfile) std::cout << "\n::- ERROR: CANNOT OPEN VERTEX SHADER FILE -::\n";
	std::ifstream FSfile(FSpath);
	if (!FSfile) std::cout << "\n::- ERROR: CANNOT OPEN FRAGMENT SHADER FILE -::\n";

	std::stringstream VSstream, FSstream;
	VSstream << VSfile.rdbuf();
	FSstream << FSfile.rdbuf();

	std::string VSstring = VSstream.str();
	std::string FSstring = FSstream.str();

	const char* VScode = VSstring.c_str();
	const char* FScode = FSstring.c_str();

	// Compile shaders
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	GLint result = 0;

	glShaderSource(VS, 1, &VScode, nullptr);
	glShaderSource(FS, 1, &FScode, nullptr);

	glCompileShader(VS);
	glGetShaderiv(VS, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(VS, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(VS, length, nullptr, message);

		std::cout << "\n::- ERROR: VERTEX SHADER COMPILATION -::\n";
		std::cout << message << std::endl;
		std::cout << "----------------------------------------" << std::endl;

		delete[] message;
	}

	result = 0;
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(FS, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetShaderInfoLog(FS, length, nullptr, message);

		std::cout << "\n::- ERROR: FRAGMENT SHADER COMPILATION -::\n";
		std::cout << message << std::endl;
		std::cout << "----------------------------------------" << std::endl;

		delete[] message;
	}

	// Link program
	this->ID = glCreateProgram();

	glAttachShader(this->ID, VS);
	glAttachShader(this->ID, FS);
	glLinkProgram(this->ID);

	result = 0;
	glGetProgramiv(this->ID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(this->ID, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		glGetProgramInfoLog(this->ID, length, nullptr, message);

		std::cout << "\n::- ERROR: LINKING SHADER PROGRAM -::\n";
		std::cout << message << std::endl;

		delete[] message;
	}

	glDeleteShader(VS);
	glDeleteShader(FS);
}

Shader::~Shader()
{
	glDeleteProgram(this->ID);
}

void Shader::bind() const
{
	glUseProgram(this->ID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::uniform_1i(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(this->ID, name.c_str());

	if (location != -1)
		glUniform1i(location, value);
	else
		std::cout << "::- ERROR: CANNOT FIND UNIFORM LOCATION OF " << name << "-::" << std::endl;
}

void Shader::uniform_mat4(const std::string& name, const glm::mat4& matrice) const
{
	GLint location = glGetUniformLocation(this->ID, name.c_str());

	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrice));
	else
		std::cout << "::- ERROR: CANNOT FIND UNIFORM LOCATION OF " << name << "-::" << std::endl;
}
