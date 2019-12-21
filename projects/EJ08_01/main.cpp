/*
EJ08.01 - Simular varios objetos reales con sus materiales según la tabla en http://devernay.free.fr/cours/opengl/materials.html 
//(tener en cuenta que esos valores están calculados para luces blancas vec3(1.0)
Autor: Joan Baixauli
Ejercicio basado en: AG08.01
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

float lastFrame = 0.0f;
const float deltaTime = 0.0f;
float lastX, lastY;
bool firstMouse = true;

class Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::string name;
public:
	void setMaterial(glm::vec3, glm::vec3, glm::vec3, float, std::string);
	glm::vec3 getAmbient() { return ambient; }
	glm::vec3 getDiffuse() { return diffuse; }
	glm::vec3 getSpecular() { return specular; }
	float getShininess() { return shininess; }
	std::string getName() { return name; }

};

void Material::setMaterial(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess, std::string _name) {
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	shininess = _shininess;
	name = _name;
}

uint8_t nMat = 12;
Material* mats = new Material[nMat];

int sMod = 0;

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

void render(const Geometry& object, const Geometry& light, const Shader& s_phong, const Shader& s_light) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::vec3 lightDiffuse(1.0f, 1.0f, 1.0f);

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

	glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
	s_phong.set("normalMat", normalMat);

	s_phong.set("viewPos", camera.getPosition());

	//sizeof(mats) / sizeof(Material);

	sMod = (int(floor(lastFrame))%nMat);
	

	std::cout << "Material: "<< sMod << " " << mats[sMod].getName() << " "   << std::endl;

	
	s_phong.set("material.ambient", mats[sMod].getAmbient().x, mats[sMod].getAmbient().y, mats[sMod].getAmbient().z);
	s_phong.set("material.diffuse", mats[sMod].getDiffuse().x, mats[sMod].getDiffuse().y, mats[sMod].getDiffuse().z);
	s_phong.set("material.specular", mats[sMod].getSpecular().x, mats[sMod].getSpecular().y, mats[sMod].getSpecular().z);
	s_phong.set("material.shininess", int(mats[sMod].getShininess()*128));

	s_phong.set("light.position", lightPos);
	s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
	s_phong.set("light.diffuse", lightDiffuse);
	s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);
	
	
	object.render();
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/AG08_01/phong.vs", "../projects/AG08_01/blinn.fs");
	const Shader s_light("../projects/AG08_01/light.vs", "../projects/AG08_01/light.fs");
	const Sphere sphere(1.0f, 50, 50);

	Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	mats[0].setMaterial(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f, "esmerald");
	mats[1].setMaterial(glm::vec3(0.135f, 0.225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 0.1f, "jade");
	mats[2].setMaterial(glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435), 0.3f, "obsidian");
	mats[3].setMaterial(glm::vec3(0.25f,0.20725f,0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f, "pearl");
	mats[4].setMaterial(glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f, "ruby");
	mats[5].setMaterial(glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.297254f), glm::vec3(0.30829f, 0.306678f, 0.626959f), 0.1f, "turquoise");
	mats[6].setMaterial(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843), 0.21794872f, "brass");
	mats[7].setMaterial(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6, "chrome");
	mats[8].setMaterial(glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1, "copper");
	mats[9].setMaterial(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2, "bronze");
	mats[10].setMaterial(glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.4, "silver");
	mats[11].setMaterial(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4, "gold");
	


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

	delete[] mats;

	return 0;
}