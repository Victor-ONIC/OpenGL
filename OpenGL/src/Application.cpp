#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>





struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{
	std::ifstream file(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static GLuint compileShader(GLenum type, const std::string& source)
{
	GLuint ID = glCreateShader(type);

	const char* src = source.c_str();
	glShaderSource(ID, 1, &src, nullptr);

	glCompileShader(ID);

	// Vérification de la compilation.
	GLint result = 0;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];

		glGetShaderInfoLog(ID, length, &length, message);
		std::cout
			<< "Erreur compilation de "
			<< (type == GL_VERTEX_SHADER ? "vertex shader !" : "fragment shader !")
			<< std::endl;
		std::cout << message << std::endl;

		glDeleteShader(ID);
		delete[] message;
		return 0;
	}


	return ID;
}

static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();

	GLuint VS = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint FS = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, VS);
	glAttachShader(program, FS);

	glLinkProgram(program);
	glValidateProgram(program);  // vérifier le link ?

	glDeleteShader(VS);
	glDeleteShader(FS);

	return program;
}

void window_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}






int main()
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cout << "Erreur dans l'initialisation de GLFW !" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context.
	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutoriel OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Erreur dans la création de la fenêtre !" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's context current.
	glfwMakeContextCurrent(window);

	// Initialize GLEW. Must be called after a valid window context has been created.
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Erreur dans l'initialisation de GLEW !" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Donner à OpenGL la taille de la fenêtre
	glViewport(0, 0, 800, 600);

	// Définir la fonction à appeler lorsque la fenêtre change de taille.
	glfwSetFramebufferSizeCallback(window, window_resize);

	// Afficher la version d'OpenGL sur la console.
	std::cout << glGetString(GL_VERSION) << std::endl;






	// Sommets.
	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	// Index des sommets à dessiner.
	unsigned int index[] = {
		0, 1, 2,
		2, 3, 0
	};

	// VAO
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// VBO
	GLuint vertexBuffID;
	glGenBuffers(1, &vertexBuffID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffID);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	// EBO
	GLuint indexBuffID;
	glGenBuffers(1, &indexBuffID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	// Vertex Attribute Layout
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// Shaders
	ShaderProgramSource source = parseShader("./res/shaders/Basic.shader");
	GLuint shader = createShader(source.vertexSource, source.fragmentSource);






	// Loop until the user closes the window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Input.
		processInput(window);

		// Draw.
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		float time = static_cast<float>(glfwGetTime());
		float colorValue = std::sin(time) / 2.0f + 0.5f;
		int uni_color = glGetUniformLocation(shader, "color");
		glUniform4f(uni_color, 0.0f, colorValue, 0.0f, 1.0f);

		glBindVertexArray(vertexArrayID);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap buffers and check events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader);                  // shader program
	glDeleteBuffers(1, &vertexBuffID);        // vbo
	glDeleteBuffers(1, &indexBuffID);         // ebo
	glDeleteVertexArrays(1, &vertexArrayID);  // vao

	glfwTerminate();
	return 0;
}
