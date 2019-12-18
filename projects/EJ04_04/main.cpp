/*
EJ04.04 - Pintar el quad y hacer que las coordenadas de textura hagan zoom sobre una parte de la textura, y usar los metodos de filtrado para ver diferencias.
Autor: Joan Baixauli
Ejercicio basado en: AG04
*/
#include <glad/glad.h>

#include "engine/input.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void handleInput() {
	std::vector<std::pair<int, int>> keys = Input::instance()->getKeys();
	for (auto& key : keys) {
		std::cout << key.first << " - " << key.second << std::endl;
	}
}

uint32_t createVertexData(uint32_t* VBO, uint32_t* EBO, int index) {
	float vertices[2][32] = {
	{
//0		//Posicion				//Color				//Textura
		1.0f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   0.55f, 0.55f, //Arriba derecha
		1.0f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.55f, 0.45f, //Abajo derecha
		0.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.45f, 0.45f, //Abajo izquierda
		0.0f, 0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.45f, 0.55f, // Arriba izquierda
	}, {
//1
		0.0f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   0.55f, 0.55f, //Arriba derecha
		0.0f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.55f, 0.45f, //Abajo derecha
		-1.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.45f, 0.45f, //Abajo izquierda
		-1.0f, 0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.45f, 0.55f, // Arriba izquierda
	}
	};

	//Definimos el valor del zoom centrado entre 0.45 y 0.55

	uint32_t indices[] = {
		0, 3, 1, 
		1, 3, 2,	
		
	};

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[index]), vertices[index], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

uint32_t createTexture(const char* path, int isNearest) {
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

	if (isNearest == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Lo ponemos en nearest para ver mejor los texels GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
	}
	else {		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Y en Linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	

	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Error loading texture " << path << std::endl;
	}

	return texture;
}

void render(uint32_t VAO[], const Shader& shader, uint32_t tex[]) {
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();
	shader.set("tex_1", 0);

	for (int i = 0; i < 2; i++) {
		glBindVertexArray(VAO[i]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex[i]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	
	

	//glBindVertexArray(VAO2);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, tex2);


	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	

	
	

	
}

int main(int, char* []) {
	Window* window = Window::instance();

	stbi_set_flip_vertically_on_load(true);

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	uint32_t VBO, EBO;
	// Dibujamos dos VAOs, uno a la derecha (0) y el otro a la izquierda (1)
	uint32_t VAO[2];
	//Por cada VAO -> CreateVertexData
	for (int i = 0; i < 2; i++) {
		VAO[i] = createVertexData(&VBO, &EBO, i);
	}

	const Shader shader("../projects/EJ04_04/vertex.vs", "../projects/EJ04_04/fragment.fs");
	// Pintaremos dos texturas idénticas cambiando solamente las propiedades de filtrado
	// Con propiedad Linear (derecha)
	// Con propiedad Nearest (izquierda)
	uint32_t tex[2];
	for (int t = 0; t < 2; t++) {
		tex[t] = createTexture("../assets/textures/bricks_arrow.jpg", t); 
	}
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (window->alive()) {
		handleInput();
		render(VAO, shader, tex);
		window->frame();
	}

	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteVertexArrays(1, &VAO[1]);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &tex[0]);
	glDeleteTextures(1, &tex[1]);

	return 0;
}