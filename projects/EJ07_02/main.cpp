/*
EJ07.02 - Cambiar valores de ambient, diffuse, specular y shininess y ver como se comporta el modelo de iluminación.
Autor: Joan Baixauli
Ejercicio basado en: AG07
NOTA: Usar teclas 1,2,3,4,5 y 0 para ver cambios
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
#include <iostream>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(4.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;
bool changeParameters[5] = { false,false,false,false,false };


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
	//Teclas adicionales para cambiar atributos del shader (uniforms)
	if (input->isKeyPressed(GLFW_KEY_1)) {
		changeParameters[0] = true;
	}
	if (input->isKeyPressed(GLFW_KEY_2)) {
		changeParameters[1] = true;
	}
	if (input->isKeyPressed(GLFW_KEY_3)) {
		changeParameters[2] = true;
	}
	if (input->isKeyPressed(GLFW_KEY_4)) {
		changeParameters[3] = true;
	}
	if (input->isKeyPressed(GLFW_KEY_5)) {
		changeParameters[4] = true;
	}
	if (input->isKeyPressed(GLFW_KEY_0)) {
		for (uint8_t i = 0; i < sizeof(changeParameters)/sizeof(bool); ++i) {
			changeParameters[i] = false;
		}
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

void render(const Geometry& object, const Geometry& light, const Shader& s_phong, const Shader& s_light) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	//Cambiar valores de ambient, diffuse, specular y shininess
	float speed = 0.5f;
	float timeLapse = lastFrame * speed;
	float ambientStrength = 0.2f;
	int shininess = 64;
	float specularStregnth = 0.6f;

	lightColor = { 1.0f, 1.0f, 1.0f };

	//Con las TECLAS 1 2 3 4 5 cambiamos los valores (Color,Pos-Diffuse, Ambient,Shininees, Specular) conforme el tiempo
	//Con la TECLA 0 reseteamos

	if (changeParameters[0]) { //Color -> 1	
		float r_ = sin(timeLapse) / 2.0f + 0.5f;
		float g_ = cos(timeLapse) / 2.0f + 0.5f;
		float b_ = (1 - r_);
		lightColor = { r_, g_, b_ };
	}
	if (changeParameters[1]) { //ambient -> 2
		ambientStrength = sin(timeLapse) / 2.0f + 0.5f;
	}
	if (changeParameters[2]) { //shininess -> 3
		int p = (int(floor(lastFrame)) % 8);
		shininess = pow(2, p+1);
		//std::cout << shininess << std::endl;
	}
	if (changeParameters[3]) { //specular -> 4
		specularStregnth = sin(timeLapse) / 2.0f + 0.5f;
	}
	if (changeParameters[4]) { //Pos -> 5
		float distanceLight = (sin(timeLapse) / 2.0f + 2.5f) * 2;
		lightPos.x = cos(glfwGetTime()) * distanceLight;
		lightPos.y = 0;
		lightPos.z = 0;
	}
	

	s_light.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.25f));
	s_light.set("model", model);
	s_light.set("view", view);
	s_light.set("proj", proj);
	s_light.set("lightColor", lightColor);

	light.render();

	s_phong.use();
	model = glm::mat4(1.0f);
	s_phong.set("model", model);
	s_phong.set("view", view);
	s_phong.set("proj", proj);


	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	s_phong.set("objectColor", glm::vec3(0.6f, 0.5f, 0.2f));
	s_phong.set("lightColor", lightColor);

	s_phong.set("ambientStrength", ambientStrength); //Cambiar ambient
	s_phong.set("lightPos", lightPos);

	s_phong.set("viewPos", camera.getPosition());
	s_phong.set("shininess", shininess); //Cambiar shininess
	s_phong.set("specularStrength", specularStregnth); //Cambiar specular

	object.render();
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/EJ07_02/phong.vs", "../projects/EJ07_02/blinn.fs");
	const Shader s_light("../projects/EJ07_02/light.vs", "../projects/EJ07_02/light.fs");
	const Sphere sphere(1.0f, 50, 50);

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
		render(sphere, sphere, s_phong, s_light);
		window->frame();
	}

	return 0;
}