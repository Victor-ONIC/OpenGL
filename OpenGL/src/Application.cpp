#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>





// Structure contenant les codes sources des shaders.
struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

// Lire le fichier "filepath" et en extraire les codes sources des shaders.
static struct ShaderProgramSource parseShader(const std::string& filepath)
{
	// Ouvrir le fichier à l'emplacement "filepath".
	std::ifstream file(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	// Lire le fichier ligne par ligne.
	std::string line;					 // string pour contenir la ligne lue actuellement.
	std::stringstream ss[2];			 // 2 string streams pour contenir le code source des shaders.
	ShaderType type = ShaderType::NONE;  // type du shader lu.

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

// Créer et compiler le shader de type "type" avec le code source "source".
static GLuint compileShader(GLenum type, const std::string& source)
{
	// Déclarer un shader de type "type", dont l'ID sera mise dans "ID".
	GLuint ID = glCreateShader(type);

	// Ajouter au shader d'ID "ID" "1" code source situé dans "src".
	const char* src = source.c_str();
	glShaderSource(ID, 1, &src, nullptr);

	// Compiler le shader d'ID "ID".
	glCompileShader(ID);

	// VÉRIFICATION
	// Obtenir le "status de la compilation" du shader "ID" et le mettre dans "result".
	GLint result = 0;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// Obtenir la "taille du log" du shader "ID" et la mettre dans "length".
		int length = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

		// Allouer assez de place pour stocker le message.
		char* message = new char[length];

		// Obtenir le message de log du shader "ID".
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

// Crée le shader final compilé et lié. Contient Vertex Shader & Fragment Shader.
static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Crée un Programme dont l'ID sera mise dans "program".
	GLuint program = glCreateProgram();

	// VS et FS sont les ID des Vertex et Fragment shaders compilés.
	GLuint VS = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint FS = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// On attache les shaders "VS" et "FS" au Programme "program".
	glAttachShader(program, VS);
	glAttachShader(program, FS);

	// On link le Programme "program". On vérifie que tout s'est bien passé.
	glLinkProgram(program);
	glValidateProgram(program);

	// Une fois que le programme final est obtenu, on peut supprimer les intermédiaires.
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
	float positions[] = {
		-0.5, -0.5,  // Sommet 0
		 0.5, -0.5,  // Sommet 1
		 0.5,  0.5,  // Sommet 2
		-0.5,  0.5,  // Sommet 3
	};

	// Tableau d'index de sommets / Vertex Indices
	unsigned int index[] = {
		0, 1, 2,  // Premier triangle
		2, 3, 0   // Deuxième triangle
	};

	// BUFFER DE SOMMETS / VERTEX BUFFER
	// Déclarer un buffer.
	GLuint vertexBuffID;
	glGenBuffers(1, &vertexBuffID);
	// Attacher le buffer à GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffID);
	// Initialiser la mémoire du buffer attaché à "GL_ARRAY_BUFFER" dans la carte graphique.
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	// BUFFER D'INDICES / INDEX BUFFER
	// Déclarer un second buffer.
	GLuint indexBuffID;
	glGenBuffers(1, &indexBuffID);
	// Attacher le buffer à GL_ELEMENT_ARRAY_BUFFER.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffID);
	// Initialiser la mémoire du buffer attaché à "GL_ELEMENT_ARRAY_BUFFER" dans la carte graphique.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	// DÉFINIR LA DISPOSITION DES DONNÉES DE SOMMET / VERTEX ATTRIBUTE LAYOUT
	// Définit l'attribut de sommet d'index "0" (ici la position) composé de "2" "GL_FLOAT" "non normalisés".
	// La taille d'un **sommet** est "2*sizeof(float)".
	// Le décalage entre cet attribut et le début du buffer est de "0". Cast en void* obligatoire...
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
	// Active l'attributs de sommet d'index "0", qui est désactivé de base.
	glEnableVertexAttribArray(0);

	// Obtenir le code source des shaders. LE CHEMIN DU FICHIER N'EST PAS TOUJOURS LE MÊME.
	ShaderProgramSource source = parseShader("./res/shaders/Basic.shader");

	// Crée un programme.
	GLuint shader = createShader(source.vertexSource, source.fragmentSource);
	// Utiliser le programme.
	glUseProgram(shader);






	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// DESSINER A PARTIR DU BUFFER GL_ELEMENT_ARRAY_BUFFER avec les indices de sommets.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Supprimer tous les objets une fois qu'ils ne sont plus utilisés.
	glDeleteProgram(shader);
	glDeleteBuffers(1, &vertexBuffID);

	glfwTerminate();

	return 0;
}
