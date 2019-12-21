/*
EJ07.05 - Montar una escena con tres esferas y una luz, la primera esfera tiene un modelo de iluminación faceteado (solo difuso),
la segunda tiene un modelo Gouraud y la tercera un modelo Phong (que acabe viendose algo parecido a la imagen adjunta).
Autor: Joan Baixauli
Ejercicio basado en: AG07
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"
#include "engine/geometry/quad.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(5.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 positions[3] = { glm::vec3(-2.5f,0.0f,-4.5f), glm::vec3(0.0f,0.0f,-4.5f), glm::vec3(2.5f,0.0f,-4.5f) };

float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
	Input* input = Input::instance();

	if (input->isKeyPressed(GLFW_KEY_W)) {
		camera.handleKeyboard(Camera::Movement::Forward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		camera.handleKeyboard(Camera::Movement::Backward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		camera.handleKeyboard(Camera::Movement::Left, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		camera.handleKeyboard(Camera::Movement::Right, dt);
	}
}

void onKeyPress(int key, int action) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(true);
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(false);
	}
}

void onMouseMoved(float x, float y) {
	if (firstMouse) {
		firstMouse = false;
		lastX = x;
		lastY = y;
	}

	const float xoffset = x - lastX;
	const float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
	camera.handleMouseScroll(y);
}

void render(const Geometry& object1, const Geometry& object2, const Geometry& object3, const Geometry& light, const Shader& s_phong, const Shader& s_gouraud, const Shader& s_flat, const Shader& s_light) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);


	s_light.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.25f));
	s_light.set("model", model);
	s_light.set("view", view);
	s_light.set("proj", proj);
	s_light.set("lightColor", lightColor);

	light.render();

	//Flat
	s_flat.use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, positions[0]);
	s_flat.set("model", model);
	s_flat.set("view", view);
	s_flat.set("proj", proj);

	s_flat.set("objectColor", glm::vec3(0.0f, 0.0f, 0.8f));
	s_flat.set("lightColor", lightColor);
	s_flat.set("lightPos", lightPos);
	//s_flat.set("ambientStrength", 0.20f); //Probado con y sin ambient (lo dejamos "sin", ya que es lo que pone el enunciado "solo difuso")
	
	object1.render();

	//Gouraud
	s_gouraud.use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, positions[1]);
	s_gouraud.set("model", model);
	s_gouraud.set("view", view);
	s_gouraud.set("proj", proj);

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_gouraud.set("normalMat", normalMat);


	s_gouraud.set("lightColor", lightColor);

	s_gouraud.set("ambientStrength", 0.20f);
	s_gouraud.set("lightPos", lightPos);

	s_gouraud.set("viewPos", camera.getPosition());
	s_gouraud.set("shininess", 64);
	s_gouraud.set("specularStrength", 0.6f);

	s_gouraud.set("objectColor", glm::vec3(0.0f, 0.0f, 0.8f));

	object2.render();

	//Phong
	s_phong.use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, positions[2]);
	s_phong.set("model", model);
	s_phong.set("view", view);
	s_phong.set("proj", proj);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	s_phong.set("objectColor", glm::vec3(0.0f, 0.0f, 0.8f));
	s_phong.set("lightColor", lightColor);

	s_phong.set("ambientStrength", 0.20f);
	s_phong.set("lightPos", lightPos);

	s_phong.set("viewPos", camera.getPosition());
	s_phong.set("shininess", 64);
	s_phong.set("specularStrength", 0.6f);

	object3.render();
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/EJ07_05/phong.vs", "../projects/EJ07_05/blinn.fs");

	const Shader s_gouraud("../projects/EJ07_05/Gouraud.vs", "../projects/EJ07_05/Gouraud.fs");

	const Shader s_flat("../projects/EJ07_05/flat.vs", "../projects/EJ07_05/flat.fs");


	const Shader s_light("../projects/EJ07_05/light.vs", "../projects/EJ07_05/light.fs");


	const Sphere sphere(1.0f, 20, 20);

	Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Input::instance()->setKeyPressedCallback(onKeyPress);
	Input::instance()->setMouseMoveCallback(onMouseMoved);
	Input::instance()->setScrollMoveCallback(onScrollMoved);

	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(deltaTime);
		render(sphere, sphere, sphere, sphere, s_phong, s_gouraud, s_flat,  s_light);
		window->frame();
	}

	return 0;
}