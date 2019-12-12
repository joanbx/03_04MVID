/*
EJ2_01 - Pintar dos triángulos, uno al lado del otro (sin formar un quad, de esta forma " //\ "), usando glDrawArrays, añadiendo más vértices a los datos (usando un solo VBO).
Autor: Joan Baixauli
Ejercicio basado en: AG02
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/input.hpp"
#include "engine/window.hpp"

#include <iostream>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

bool checkShader(uint32_t shader) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Error Compiling Shader " << infoLog << std::endl;
		return false;
	}
	return true;
}

bool checkProgram(uint32_t program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "Error Linking Program " << infoLog << std::endl;
		return false;
	}
	return true;
}

uint32_t createProgram() {
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location=0) in vec3 aPos;\n"
		"void main() {\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"    FragColor = vec4(0.6, 0.6, 0.1, 1.0);\n"
		"}\0";

	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	checkShader(vertexShader);

	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader);

	const uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	checkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

uint32_t createVertexData(uint32_t* VBO) {


	float verticesAll[] = {
		// Primer triangulo
		-1.0f, -0.5f, 0.0f, // Izq
		-0.0f, -0.5f, 0.0f, // Der
		-0.5f, 0.5f, 0.0f, // Arriba
		// Segundo triangulo
		0.0f, -0.5f, 0.0f, // Izq
		1.0f, -0.5f, 0.0f, // Der
		0.5f, 0.5f, 0.0f // Arriba
	};

	uint32_t VAO;

	glGenVertexArrays(1, &VAO); //Creamos VAO
	glGenBuffers(1, VBO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);

	// 2. Copiar nuestro array de vértices en un buffer para ser usado por OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAll), verticesAll, GL_STATIC_DRAW);

	// 3. Entonces ponemos nuestros "Vertex attribute pointers"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind VBO
	glBindVertexArray(0); //Unbind VAO

	return VAO;
}


void render(uint32_t VAO, uint32_t program) {
	glClear(GL_COLOR_BUFFER_BIT);
	//Dibujamos primitivos usando el shader actual que esté activo, el "Vertex attribute configuration" y el VBO vertex data (indirectamente vinculado via el VAO)
	glUseProgram(program);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0,6);
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO;
	const uint32_t VAO = createVertexData(&VBO);

	const uint32_t program = createProgram();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();
		render(VBO, program);
		window->frame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteProgram(program);

	return 0;
}