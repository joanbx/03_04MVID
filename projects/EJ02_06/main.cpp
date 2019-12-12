/*
EJ02.06 - Pintar el mismo hexágono, pero generando los vértices de manera procedural (usando algún tipo de algoritmo), mediante coordenadas polares.
Autor: Joan Baixauli
Ejercicio basado en: AG02
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/input.hpp"
#include "engine/window.hpp"

#include <iostream>

#include <cmath>
#include <vector> 

#define PI 3.14159265

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

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO, int numVertices) {
	
	/*
		Nos basamos en esta imágen de Hexágono usando polares desde su punto céntrico: 
		https://upload.wikimedia.org/wikipedia/commons/thumb/e/e2/Sechseck-Zeichnung.svg/220px-Sechseck-Zeichnung.svg.png

	*/
	
	if (numVertices < 3) numVertices = 3;				//Mínimo de 3 vertices (triangulo)
	else if(numVertices > 360) numVertices = 360;		//Máximo de 360 vertices (virtualmente es un círculo para una resolución no HD o superior)

	const float ang = 360 / numVertices;				//El ángulo o coordenada polar (theta) que separa cada radio (Ru)

	std::cout << "Poligono de " << numVertices << " caras (vértices) y angulo de: " << ang << " grados " << std::endl;

	//usaremos variables "vector" para hacer un array dinámico de forma fácil
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	//El primer vector3 es (0,0,0): punto céntrico
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	//Empezando con Theta 0...
	float angi = 0;
		
	for (int i = 0; i < numVertices; i++) {
		//Calculamos los valores cartesianos a partir del valor polar correspondiente
		vertices.push_back(cos(angi * PI / 180.0f));	//x
		vertices.push_back (sin(angi * PI / 180.0f));	//y
		vertices.push_back(0.0f);						//z

		//Siguiente ángulo:
		angi += ang;
	}

	for (int j = 0; j < numVertices; j++) {
		//Vamos añadiendo los valores al indice correspondiente a cada triángulo 0,1,2, 0,2,3, ... 0,n,1
		int k = j + 1;
		int l = j + 2;
		
		if (l> numVertices) {
			k = numVertices;
			l = 1;
		}

		indices.push_back(0); //Vértice céntrico
		indices.push_back(k); //Segundo vértice triángulo
		indices.push_back(l); //Tercer vértice triángulo

	}

	
	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}


void render(uint32_t VAO, uint32_t program, int trinum) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, trinum, GL_UNSIGNED_INT, nullptr);
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	//Sustituye su valor por un numero entre 3 y 360 para obtener el correspondiente polígono. Default: 6 (Hexágono)
	//------
	const int VERTEXNUM = 6; 
	//------
	//Nota: Las COORDENADAS POLARES se calculan a partir de VERTEXNUM dividiendo 360 por dicho número (Ver createVertexData)

	uint32_t VBO, EBO;
	const uint32_t VAO = createVertexData(&VBO, &EBO, VERTEXNUM);
	const uint32_t program = createProgram();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();
		render(VAO, program, VERTEXNUM *3);
		window->frame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteProgram(program);

	return 0;
}