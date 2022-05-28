#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

// Cr�er et compiler le shader de type "type" avec le code source "source".
static GLuint compileShader(GLenum type, const std::string& source)
{
	// D�clarer un shader de type "type", dont l'ID sera mise dans "ID".
	GLuint ID = glCreateShader(type);

	// Ajouter au shader d'ID "ID" "1" code source situ� dans "&src".
	const char* src = source.c_str();
	glShaderSource(ID, 1, &src, nullptr);

	// Compiler le shader d'ID "ID".
	glCompileShader(ID);

	// V�RIFICATION
	// Obtenir le "status de la compilation" du shader "ID" et le mettre dans "&result".
	GLint result = 0;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// Obtenir la "taille du log" du shader "ID" et la mettre dans "&length".
		int length = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

		// Allouer assez de place sur la pile pour stocker le message.
		char* message = (char*)alloca(length * sizeof(char));

		// Obtenir le message de log du shader "ID".
		glGetShaderInfoLog(ID, length, &length, message);
		std::cout
			<< "Erreur compilation de "
			<< (type == GL_VERTEX_SHADER ? "vertex shader !" : "fragment shader !")
			<< std::endl;
		std::cout << message << std::endl;

		glDeleteShader(ID);
		return 0;
	}


	return ID;
}

// Cr�e le shader final compil� et li�. Contient Vertex Shader & Fragment Shader.
static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Cr�e un Programme dont l'ID sera mise dans "program".
	GLuint program = glCreateProgram();

	// VS et FS sont les ID des Vertex et Fragment shaders compil�s.
	GLuint VS = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint FS = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// On attache les shaders "VS" et "FS" au Programme "program".
	glAttachShader(program, VS);
	glAttachShader(program, FS);

	// On link le Programme "program". On v�rifie que tout s'est bien pass�.
	glLinkProgram(program);
	glValidateProgram(program);

	// Une fois que le programme final est obtenu, on peut supprimer les interm�diaires.
	glDeleteShader(VS);
	glDeleteShader(FS);

	return program;
}

int main()
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cout << "Erreur glfwInit() !" << std::endl;
		return -1;
	}

	// Create a windowed mode window and its OpenGL context.
	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "Tutoriel OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Erreur glfwCreateWindow() !" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's context current.
	glfwMakeContextCurrent(window);

	// Initialize GLEW.
	// Must be called after a valid window context has been created.
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Erreur glewInit() !" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Afficher la version d'OpenGL sur la console.
	std::cout << glGetString(GL_VERSION) << std::endl;






	// Tableau de sommets / Vertex array
	// Ici, 3 sommets d'1 attribut: position (2 floats).
	float positions[6] = {
		-0.5, -0.5,  // Sommet 1: position (x, y), et c'est tout.
		 0.0,  0.5,  // Sommet 2: position (x, y), et c'est tout.
		 0.5, -0.5   // Sommet 3: position (x, y), et c'est tout.
	};

	// D�clarer "1" buffer dont l'ID sera mise dans "&buffId".
	GLuint buffId = 0;
	glGenBuffers(1, &buffId);

	// Attache � "GL_ARRAY_BUFFER" le buffer d'ID "buffId".
	glBindBuffer(GL_ARRAY_BUFFER, buffId);
	// Initialiser la m�moire du buffer attach� � "GL_ARRAY_BUFFER" avec "6*sizeof(float)" octets de "positions" (copie).
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// D�FINIR LA DISPOSITION DES DONN�ES DE SOMMET / VERTEX ATTRIBUTE LAYOUT
	// D�finit l'attribut de sommet d'index "0" (ici la position) compos� de "2" "GL_FLOAT" "non normalis�s".
	// La taille d'un **sommet** est "2*sizeof(float)".
	// Le d�calage entre cet attribut et le d�but du buffer est de "0". Cast en void* obligatoire...
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
	// Active les attributs de sommets d'index "0", qui sont d�sactiv�s de base.
	glEnableVertexAttribArray(0);

	// Code source du Vertex Shader.
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	// Code source du Fragment Shader.
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	// Cr�e un programme et met son ID dans "shader".
	GLuint shader = createShader(vertexShader, fragmentShader);
	// Utiliser le programme d'ID "shader".
	glUseProgram(shader);






	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Supprimer tous les objets une fois qu'ils ne sont plus utilis�s.
	glDeleteProgram(shader);
	glDeleteBuffers(1, &buffId);

	glfwTerminate();

	return 0;
}
