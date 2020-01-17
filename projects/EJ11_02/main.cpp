/*
EJ11.02 - Modelo de iluminación Blinn, crear una escena con un suelo y cuatro quads perpendiculares alineados, uno detrás de otro. 
Uno de ellos usa mapas difusos y speculares para pintarse solido. Los otros tres se pintan de un color plano, uno rojo, otro verde y otro azul, 
y cada uno con un grado de transparencia distinto.
Autor: Joan Baixauli
Ejercicio basado en: AG11.03
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

class QuadColors {
	glm::vec3 _position;
	glm::vec3 _color;
	float _transparency;

public:
	QuadColors(glm::vec3 position, glm::vec3 color, float transparency);
	glm::vec3 getPosition() { return _position; }
	glm::vec3 getColor() { return _color; }
	float getTransparency() { return _transparency; }
};

QuadColors::QuadColors(glm::vec3 position, glm::vec3 color, float transparency) : _position(position), _color(color), _transparency(transparency) {};


//ORDEN DE LOS QUADS DE MAS LEJOS A MAS CERCA
QuadColors quadColors[] = {
	{glm::vec3(0.0f, 0.0f, -4.0f),glm::vec3(0.0f,0.0f,1.0f),0.3f},
	{glm::vec3(0.0f, 0.0f, -2.0f),glm::vec3(0.0f,1.0f,0.0f),0.5f},
	{glm::vec3(0.0f, 0.0f, -0.0f),glm::vec3(1.0f,0.0f,0.0f),0.8f}
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

void render(const Geometry& quad, const Shader& s_phong, const Shader& s_blend, const Shader& s_blendColor,
	const Texture& t_albedo, const Texture& t_specular, const Texture& t_tree) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	s_phong.use();

	//FLOOR
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

	s_phong.set("light.direction", -0.2f, -1.0f, -0.3f);
	s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
	s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
	s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);

	t_albedo.use(s_phong, "material.diffuse", 0);
	t_specular.use(s_phong, "material.specular", 1);
	s_phong.set("material.shininess", 32);

	quad.render();

	//BLEND-QUADS

	//QUAD-TEXTURE
	s_blend.use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	s_blend.set("model", model);
	s_blend.set("view", view);
	s_blend.set("proj", proj);
	t_tree.use(s_blend, "texture1", 0);
	quad.render();

	//QUAD COLOR SEMI-TRANSPARENTE
	s_blendColor.use();
	for (uint8_t i = 0; i < 3; ++i) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, quadColors[i].getPosition());
		s_blendColor.set("model", model);
		s_blendColor.set("view", view);
		s_blendColor.set("proj", proj);
		s_blendColor.set("addColor", quadColors[i].getColor());
		s_blendColor.set("transparency", quadColors[i].getTransparency());
		quad.render();
	}

}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/EJ11_02/phong.vs", "../projects/EJ11_02/blinn.fs");
	const Shader s_blend("../projects/EJ11_02/blend.vs", "../projects/EJ11_02/blend.fs");
	const Shader s_blendColor("../projects/EJ11_02/blendColor.vs", "../projects/EJ11_02/blendColor.fs");
	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	const Texture t_tree("../assets/textures/tree.png", Texture::Format::RGBA);
	const Cube cube(1.0f);
	const Quad quad(1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Input::instance()->setKeyPressedCallback(onKeyPress);
	Input::instance()->setMouseMoveCallback(onMouseMoved);
	Input::instance()->setScrollMoveCallback(onScrollMoved);

	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(deltaTime);
		render(quad, s_phong, s_blend, s_blendColor, t_albedo, t_specular, t_tree);
		window->frame();
	}

	return 0;
}