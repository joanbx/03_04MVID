/*
EJ03.02 - Cambiar el color del triángulo con el paso del tiempo, usando un uniform para ello y la función glfwGetTime(), que mide el tiempo que ha transcurrido desde que se inició GLFW.
Autor: Joan Baixauli
Ejercicio basado en: AG03
*/
#include <glad/glad.h>

#include "engine/input.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"

#include <iostream>
#include <GLFW\glfw3.h>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO) {
	float vertices[] = {
		0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f
	};
	//El valor de los colores es 0 para todos, pero podemos cambiar aqui los valor para obtener distintos efectos de colores segun pasa el tiempo (ver loop)


	uint32_t indices[] = {
		0, 2, 1
	};

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}


void render(uint32_t VAO, const Shader& shader) {
	glClear(GL_COLOR_BUFFER_BIT);
	shader.use();

	float speed = 0.5f;
	float timeLapse = glfwGetTime()*speed;
	float r_ = sin(timeLapse) / 2.0f + 0.5f;
	float g_ = cos(timeLapse) / 2.0f + 0.5f;
	float b_ = (1-r_);
	//std::cout << "r " << r_ << " g " << g_ << " b " << b_ << std::endl;
	shader.set("addColor",r_, g_, b_);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;
	const uint32_t VAO = createVertexData(&VBO, &EBO);
	const Shader shader("../projects/EJ03_02/vertex.vs", "../projects/EJ03_02/fragment.fs");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	while (window->alive()) {
		handleInput();
	
		render(VAO, shader);
		window->frame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}