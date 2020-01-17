/*
EJ09.04 - En la escena del punto 1, pintarla con una luz direccional, 2 point lights y 2 spotlights. Pintar una esfera para cada luz en la posición de cada una de ellas. Y hacer que cada luz emita un color/propiedades distintas.
Autor: Joan Baixauli
Ejercicio basado en: AG09.04
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
#include "engine/light.hpp"


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


uint8_t nPointLight = 2;
uint8_t nSpotLight = 2;

DirLight dirLight;
PointLight* pointLights = new PointLight[nPointLight]{
	{glm::vec3(4.0f,2.0f,0.0f), glm::vec3(0.2f,0.0f,0.0f), glm::vec3(0.5f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.06f, glm::vec3(1.0f,0.0f,0.0f)},
	{glm::vec3(-4.0f,2.0f,0.0f), glm::vec3(0.0f,0.2f,0.0f), glm::vec3(0.0f,0.5f,0.0f),glm::vec3(1.0f,1.0f,1.0f), 0.1f, 0.5f, 0.06f, glm::vec3(0.0f,1.0f,0.0f)}
};

SpotLight* spotLights = new SpotLight[nSpotLight]{
	{glm::vec3(0.0f,2.0f,4.0f), glm::vec3(-0.5f, -1.0f,-0.5f), glm::vec3(0.0f,0.2f,0.2f), glm::vec3(0.0f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.3f, 0.16f, 50.0, 60.0, glm::vec3(0.0f,1.0f,1.0f)},
	{glm::vec3(0.0f,2.0f,-4.0f),glm::vec3 (0.5f, -1.0f, 0.5f), glm::vec3(0.2f,0.2f,0.0f), glm::vec3(0.5f,0.5f,0.0f),glm::vec3(1.0f,1.0f,1.0f), 0.5f, 0.1f, 0.32f, 20.0, 20.0, glm::vec3(1.0f,1.0f,0.0f)}
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

void render(const Geometry& floor, const Geometry& object, const Geometry& light, const Shader& s_phong, const Shader& s_light,
	const Texture& t_albedo, const Texture& t_specular) {
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
		spotLights[i].setShader(s_phong,i);	
	}
	
	t_albedo.use(s_phong, "material.diffuse", 0);
	t_specular.use(s_phong, "material.specular", 1);
	s_phong.set("material.shininess", 32);

	floor.render();

	//OBJECT
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0,0,0));
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	s_phong.set("model", model);

	normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	object.render();
	
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	const Shader s_phong("../projects/EJ09_04/phong.vs", "../projects/EJ09_04/blinn.fs");
	const Shader s_light("../projects/EJ09_04/light.vs", "../projects/EJ09_04/light.fs");
	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	const Sphere sphere(1.0f, 50, 50);
	const Teapot teapot(40);
	const Quad quad(5.0f);

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
		render(quad, teapot, sphere, s_phong, s_light, t_albedo, t_specular);
		window->frame();
	}
	delete[] pointLights;
	delete[] spotLights;
	return 0;
}