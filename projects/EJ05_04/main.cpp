/*
EJ05.04 - Crear una escena con un suelo, y varios cubos apoyados en él, a diversas alturas, distancias y tamaños
Autor: Joan Baixauli
Ejercicio basado en: AG05
*/
#include <glad/glad.h>

#include "engine/window.hpp"
#include "engine/shader.hpp"

#include "engine/texture.hpp"
#include "engine/geometry/cube.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <engine\geometry\quad.hpp>

void handleInput() {/*Intentionally Left BLank*/ }

void render(const Geometry& geom, glm::vec3 cubes[][3], const Shader& shader, Texture* tex) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	
	for (uint8_t i = 0; i < 10; i++) {
		//std::cout << cubes[i][0][0] << " " << cubes[i][0][1] << " " << cubes[i][0][2] << std::endl;
		

		glm::mat4 model = glm::mat4(1.0f);
		
		if(i!=0)
			model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), cubes[i][1]);
		else
			model = glm::rotate(model, glm::radians(0.0f), cubes[i][1]);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, cubes[i][0]);

		glm::mat4 scale = glm::mat4(1.0f);
		scale = glm::scale_slow(scale, cubes[i][2]);
		shader.use();

		if (i == 0)
			tex[0].use(shader, "tex", 0);
		else
			tex[1].use(shader, "tex", 0);

		shader.set("model", model);
		shader.set("view", view);
		shader.set("scale", scale);
		shader.set("proj", proj);  //TODO const mat4

		geom.render();
	}
	
	
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader shader("../projects/EJ05_04/vertex.vs", "../projects/EJ05_04/fragment.fs");
	const Cube cube(0.5f);
	
	glm::vec3 cubePRS[][3] = {
		{ //Cube 01 // Plano
			glm::vec3(-0.0f,-1.0f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(20.f,1.0f,20.0f), //scale
		},
		{ //Cube 02 (Encima de Cubo 01)
			glm::vec3(-0.1f,-0.5f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		},
		{ //Cube 03 (Encima de Cubo 01)
			glm::vec3(1.0f,-0.25f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(2.0f,2.0f,2.0f), //scale
		},
		{ //Cube 04 (Encima de Cubo 01)
			glm::vec3(-0.75f,-0.55f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(0.75f,0.75f,0.75f), //scale
		},
		{ //Cube 05 (Encima de Cubo 01)
			glm::vec3(-1.25f,-0.6f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(0.5f,0.5f,0.5f), //scale
		},
		{ //Cube 06 //Se nota que está detrás
			glm::vec3(0.0f,0.2f,-8.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(3.5f,3.5f,3.5f), //scale
		},
		{ //Cube 07 - Flotando
			glm::vec3(-0.0f,0.9f,-3.0f), //Position
			glm::vec3(0.5f,1.0f,0.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		},
		{ //Cube 08
			glm::vec3(1.0f,1.0f,-3.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(0.25f,0.25f,0.25f), //scale
		},
		{ //Cube 09 (se ve igual que el anterior, pero es doble de grande y esta el doble de lejos)
			glm::vec3(-2.0f,2.0f,-6.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(0.5f,0.5f,0.5f), //scale
		},
		{ //Cube 10 (se ve igual que el anterior, pero es doble de grande y esta el doble de lejos)
			glm::vec3(-4.0f,0.5f,-12.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		}
	};
	glEnable(GL_DEPTH_TEST); //IMPORTANTE!
	
	Texture textures[2]{
		Texture("../assets/textures/grey.png", Texture::Format::RGB),
		Texture ("../assets/textures/blue_blocks.jpg", Texture::Format::RGB),
		//Texture ("../assets/textures/EJ05.02_cube_uv_b.jpg", Texture::Format::RGB)

	};

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Window::instance()->setCaptureMode(false);

	while (window->alive()) {
		handleInput();
		render(cube, cubePRS, shader, textures);
		window->frame();
	}

	return 0;
}