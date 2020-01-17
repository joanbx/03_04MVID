/*
EJ10.01 - Importar un modelo 3D complejo a elección (de algún repositorio de modelos 3D gratis). El modelo tiene que tener lo necesario para pintarlo correctamente. Para pintar hay que usar el modelo de iluminación Blinn, mapas difusos y speculares, y usar al menos una luz direccional, y una point light.
Autor: Joan Baixauli
Ejercicio basado en: AG10
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
#include "engine/geometry/teapot.hpp"
#include "engine/model.hpp"
#include "engine/light.hpp"
#include <iostream>

Camera camera(glm::vec3(0.0f, 1.0f, 13.0f));


uint8_t nPointLight = 3;
uint8_t nSpotLight = 3;

DirLight dirLight;
PointLight* pointLights = new PointLight[nPointLight]{
	{glm::vec3(0.0f,6.0f,4.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.06f, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(-2.0f,6.0f,6.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.06f, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(2.0f,6.0f,6.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.06f, glm::vec3(1.0f,1.0f,1.0f)}
};

SpotLight* spotLights = new SpotLight[nSpotLight]{
	{glm::vec3(-1.58f,2.3f,8.15f), glm::vec3(0.0f, 0.0f,1.0f), glm::vec3(0.0f,0.2f,0.2f), glm::vec3(0.0f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 10.0, 20.0, glm::vec3(0.0f,1.0f,1.0f)},
	{glm::vec3(1.58f,2.3f,8.15f),glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,0.2f,0.2f), glm::vec3(0.0f,0.8f,0.8f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 10.0, 20.0, glm::vec3(0.0f,1.0f,1.0f)},
	{glm::vec3(0.0f,1.55f,-4.15f),glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f,0.0f,0.0f), glm::vec3(0.5f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,0.0f,0.0f)}
};



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

void render(const Geometry& floor, const Model& object, const Geometry& light, const Shader& s_phong, const Shader& s_light, const Texture& t_albedo, const Texture& t_specular) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	//LIGHT POSITIONS
	s_light.use();
	//POINTLIGHTS
	for (uint32_t i = 0; i < nPointLight; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLights[i].getPosition());
		model = glm::scale(model, glm::vec3(0.25f));
		s_light.set("model", model);
		s_light.set("view", view);
		s_light.set("proj", proj);
		s_light.set("lightColor", pointLights[i].getColorSphere());

		light.render();
	}
	//SPOTLIGHTS
	for (uint32_t i = 0; i < nSpotLight; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, spotLights[i].getPosition());
		model = glm::scale(model, glm::vec3(0.25f));
		s_light.set("model", model);
		s_light.set("view", view);
		s_light.set("proj", proj);
		s_light.set("lightColor", spotLights[i].getColorSphere());

		light.render();
	}


	//SHADER PHONG
	s_phong.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	s_phong.set("model", model);
	s_phong.set("view", view);
	s_phong.set("proj", proj);

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	s_phong.set("viewPos", camera.getPosition());

	//DIRLIGHT
	s_phong.set("dirLight.direction", dirLight.getDirection());
	s_phong.set("dirLight.ambient", dirLight.getAmbient());
	s_phong.set("dirLight.diffuse", dirLight.getDiffuse());
	s_phong.set("dirLight.specular", dirLight.getSpecular());

	//POINTLIGHT
	for (uint32_t i = 0; i < nPointLight; ++i) {
		pointLights[i].setShader(s_phong, i);
	}

	//SPOTLIGHT
	for (uint32_t i = 0; i < nSpotLight; ++i) {
		spotLights[i].setShader(s_phong, i);
	}

	t_albedo.use(s_phong, "material.diffuse", 0);
	t_specular.use(s_phong, "material.specular", 1);
	s_phong.set("material.shininess", 32);

	floor.render();

	//OBJECT IMPORTED
	model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	s_phong.set("model", model);
	s_phong.set("view", view);
	s_phong.set("proj", proj);

	

	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	object.render(s_phong);
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	const Shader s_phong("../projects/EJ10_01/phong.vs", "../projects/EJ10_01/blinn.fs");
	const Shader s_light("../projects/EJ10_01/light.vs", "../projects/EJ10_01/light.fs");
	const Model object("../assets/models/Sci_Fi_Fighter_Ship_v1/13897_Sci-Fi_Fighter_Ship_v1_l1.obj");

	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	const Sphere sphere(1.0f, 50, 50);
	const Quad quad(5.0f);


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
		render(quad, object, sphere, s_phong, s_light, t_albedo, t_specular);
		window->frame();
	}
	delete[] pointLights;
	delete[] spotLights;
	return 0;
}