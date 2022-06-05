#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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
	if (!glfwInit())
	{
		std::cout << "Erreur dans l'initialisation de GLFW !" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutoriel OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Erreur dans la création de la fenêtre !" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Erreur dans l'initialisation de GLEW !" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, window_resize);

	std::cout << glGetString(GL_VERSION) << std::endl;






	// Sommets.
	float positions[] = {
		 // position    // color            // texture coords
		 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // haut droit
		 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bas droit
		-0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bas gauche
		-0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // haut gauche
	};

	// Index des sommets à dessiner.
	unsigned int index[] = {
		0, 1, 3,
		3, 2, 1
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Shaders
	Shader shader("./res/shaders/vertex_shader.vert", "./res/shaders/fragment_shader.frag");

	// Texture
	stbi_set_flip_vertically_on_load(true);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nb_channels;
	unsigned char* texture_data = stbi_load("./res/textures/container.jpg", &width, &height, &nb_channels, 0);
	if (texture_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "\n::-FAILED TO LOAD TEXTURE-::\n";
	}
	stbi_image_free(texture_data);






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
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(vertexArrayID);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap buffers and check events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexBuffID);        // vbo
	glDeleteBuffers(1, &indexBuffID);         // ebo
	glDeleteVertexArrays(1, &vertexArrayID);  // vao
	glDeleteTextures(1, &textureID);          // texture

	glfwTerminate();
	return 0;
}
