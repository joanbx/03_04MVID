/*
EJ08.02 - Cambiar valores de ambient, diffuse, specular de la luz y ver como afecta a como se ve el cubo.
Autor: Joan Baixauli
Ejercicio basado en: AG08.02
Nota: Usar teclas 1,2,3 y 0 para ver cambios
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
glm::vec3 lightPos(4.0f, 1.0f, 0.0f);

float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

bool changeParameters[3] = { false,false,false };

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
	if (input->isKeyPressed(GLFW_KEY_0)) {
		for (uint8_t i = 0; i < sizeof(changeParameters) / sizeof(bool); ++i) {
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

void render(const Geometry& object, const Geometry& light, const Shader& s_phong, const Shader& s_light,
	const Texture& t_albedo, const Texture& t_specular) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 lightAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

	s_light.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.25f));
	s_light.set("model", model);
	s_light.set("view", view);
	s_light.set("proj", proj);
	s_light.set("lightColor", lightDiffuse);

	light.render();

	s_phong.use();
	model = glm::mat4(1.0f);
	s_phong.set("model", model);
	s_phong.set("view", view);
	s_phong.set("proj", proj);

	if (changeParameters[0] || changeParameters[1] || changeParameters[2]) { //Cambiamos parametros con el tiempo
		float r_ = sin(lastFrame) / 2.0f + 0.5f;
		float g_ = cos(lastFrame) / 2.0f + 0.5f;
		float b_ = (1 - r_);
		if (changeParameters[0]) { //Diffuse -> 1

			lightDiffuse = { r_, g_, b_ };
		}
		if (changeParameters[1]) { //ambient -> 2

			lightAmbient = { r_, g_, b_ };
		}
		if (changeParameters[2]) { //Specular -> 3

			lightSpecular = { r_, g_, b_ };
		}

	}


	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	s_phong.set("viewPos", camera.getPosition());

	s_phong.set("light.position", lightPos);
	s_phong.set("light.ambient", lightAmbient);
	s_phong.set("light.diffuse", lightDiffuse);
	s_phong.set("light.specular", lightSpecular);

	t_albedo.use(s_phong, "material.diffuse", 0);
	t_specular.use(s_phong, "material.specular", 1);
	s_phong.set("material.shininess", 32);

	object.render();
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/EJ08_02/phong.vs", "../projects/EJ08_02/blinn.fs");
	const Shader s_light("../projects/EJ08_02/light.vs", "../projects/EJ08_02/light.fs");
	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
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
		render(sphere, sphere, s_phong, s_light, t_albedo, t_specular);
		window->frame();
	}

	return 0;
}