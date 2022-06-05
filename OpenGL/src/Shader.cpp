#include "Shader.h"

Shader::Shader(const std::string& VSpath, const std::string& FSpath)
{
	// Get shaders code
	std::ifstream VSfile(VSpath);
	if (!VSfile) std::cout << "\n::-ERREUR OUVERTURE FICHIER VERTEX SHADER-::\n";
	std::ifstream FSfile(FSpath);
	if (!FSfile) std::cout << "\n::-ERREUR OUVERTURE FICHIER FRAGMENT SHADER-::\n";

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

		std::cout << "\n::-ERREUR COMPILATION VERTEX SHADER-::\n";
		std::cout << message << std::endl;

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

		std::cout << "\n::-ERREUR COMPILATION FRAGMENT SHADER-::\n";
		std::cout << message << std::endl;

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

		std::cout << "\n::-ERROR LINKING PROGRAM-::\n";
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

void Shader::bind()
{
	glUseProgram(this->ID);
}

void Shader::unbind()
{
	glUseProgram(0);
}
