/*
EJ2_02 - Pintar los dos mismos triángulos usando dos VAOs y VBOs diferentes para sus datos.
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



uint32_t createVertexDataAll(uint32_t* VBO, int tri) {

	//Creamos una matriz con dos datos diferentes, uno por cada triangulo que queremos pintar
	float TriangleAll[2][9] = { 
		//Triangulo Uno
		{
			-1.0f, -0.5f, 0.0f, // Izq
			0.0f, -0.5f, 0.0f, // Der
			-0.5f, 0.5f, 0.0f, // Arriba
		},
		//Triangulo Dos
		{
			0.0f, -0.5f, 0.0f, // Izq
			1.0f, -0.5f, 0.0f, // Der
			0.5f, 0.5f, 0.0f // Arriba
		} 
	};

	uint32_t VAO;

	glGenVertexArrays(1, &VAO); //Creamos VAO
	glGenBuffers(1, VBO);

	glBindVertexArray(VAO); //Bind VAO


	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleAll[tri]), TriangleAll[tri], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind VBO
	glBindVertexArray(0); //Unbind VAO

	return VAO;
}



void render(uint32_t VAO[] , uint32_t program) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	//Por cada VAO de la array -> drawArrays
	for (int i = 0; i < sizeof(VAO); i++) {
		glBindVertexArray(VAO[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	int trilength = 2;

	uint32_t VAOs[2],VBOs[2]; //Aqui tenemos dos VAOs y dos VBOs

	//Por cada VAO/VBO -> CreateVertexData
	for (int i = 0; i < trilength; i++) {
		VAOs[i] = createVertexDataAll(&VBOs[i], i);
	}
	
	

	const uint32_t program = createProgram();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();
		render(VAOs, program);

		window->frame();
	}

	for (int i = 0; i < trilength; i++) {
		glDeleteVertexArrays(1, &VAOs[i]);
		glDeleteVertexArrays(1, &VBOs[i]);
	}
	

	glDeleteProgram(program);

	return 0;
}