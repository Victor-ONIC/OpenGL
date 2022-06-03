#include <GL/glew.h>  // Fonctions OpenGL
#include <GLFW/glfw3.h>  // Fenêtres

#include <iostream>

#include "Shader.h"

void window_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
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
		 0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	// Index des sommets à dessiner.
	unsigned int index[] = {
		0, 1, 2
	};

	// VAO
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// VBO
	GLuint vertexBuffID;
	glGenBuffers(1, &vertexBuffID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// EBO
	GLuint indexBuffID;
	glGenBuffers(1, &indexBuffID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	// Vertex Attribute Layout
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Shaders
	Shader shader("./res/shaders/vertex_shader.vert", "./res/shaders/fragment_shader.frag");






	// Loop until the user closes the window
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Input.
		process_input(window);

		// Draw.
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		glBindVertexArray(vertexArrayID);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		// Swap buffers and check events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexBuffID);        // vbo
	glDeleteBuffers(1, &indexBuffID);         // ebo
	glDeleteVertexArrays(1, &vertexArrayID);  // vao

	glfwTerminate();
	return 0;
}
