/*
EJ10.01 - Importar un modelo 3D complejo a elección (de algún repositorio de modelos 3D gratis). El modelo tiene que tener lo necesario para pintarlo correctamente. Para pintar hay que usar el modelo de iluminación Blinn, mapas difusos y speculares, y usar al menos una luz direccional, y una point light.
Autor: Joan Baixauli
Ejercicio basado en: AG10
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
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
#include "engine/gameObject.hpp"
//#include "engine/assets.hpp"
//#include "engine/node.hpp"
#include <iostream>
//#include <engine\sceneGraph.hpp>

Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
glm::vec3 posShip(0.0f, 3.0f, 0.0f);
glm::vec3 rotShip(0.0f, 0.0f, 0.0f);
float angleShip = 0.0f;
uint8_t nPointLight = 3;
uint8_t nSpotLight = 3;

bool enemyAlive = true;

//BULLETS
bool shoot = false;
std::vector<glm::vec3>  posBullets;
glm::vec3 forwardBullet(0.0f, 0.0f, -1.0f);

//Floor
//GameObject GOfloor(glm::vec3(0.0f, -0.5f, 0.0f));



//Create Lights

//PointLight* pointLights = new PointLight[nPointLight]{
//	{glm::vec3(0.0f,2.0f,0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)},
//	{glm::vec3(3.0f,2.0f,2.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)},
//	{glm::vec3(-3.0f,2.0f,-2.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)}
//
//};
//
//SpotLight* spotLights = new SpotLight[nSpotLight]{
//	{glm::vec3(-1.0f,0.25f,0.0f), glm::vec3(0.0f, -1.0f,0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)},
//	{glm::vec3(1.0f,0.25f,0.0f),glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)},
//	{glm::vec3(0.0f,0.25f,0.0f),glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)}
//};

std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;
//SpotLight* spotLights;


enum class Movement {
	Forward = 0,
	Backward = 1,
	Left = 2,
	Right = 3,
};

void ShipMovement(Movement direction, float dt) {
	const float velocity = 1 * dt;
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::vec3(1, 0, 0);

	switch (direction) {
		case Movement::Forward: posShip += front * velocity; break;
		case Movement::Backward: posShip -= front * velocity; break;
		case Movement::Left: posShip -= right * velocity; break;
		case Movement::Right: posShip += right * velocity; break;
		default:;
	}
}



void ShipDirection(Movement direction, float dt) {
	const float velocity = 10 * dt;
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::vec3(1, 0, 0);

	float angleMAX = 30.0f;
	float _angleShip = 1.0f;

	switch (direction) {
		/*case Movement::Forward:
		rotShip = glm::vec3(1, 0, 0);
		angleShip += _angleShip * velocity;
		if (angleShip > angleMAX) angleShip = angleMAX;
		break;
	case Movement::Backward:
		rotShip = glm::vec3(1, 0, 0);
		angleShip -= _angleShip * velocity;
		if (angleShip < -angleMAX) angleShip = -angleMAX;
		break;*/
	case Movement::Left:
		rotShip = glm::vec3(0, 1, 0);
		angleShip += _angleShip * velocity;
		if (angleShip > angleMAX) angleShip = angleMAX;
		break;
	case Movement::Right:
		rotShip = glm::vec3(0, 1, 0);
		angleShip -= _angleShip * velocity;
		if (angleShip < -angleMAX) angleShip = -angleMAX;
		break;
	default:;
	}


	std::cout << angleShip << " " << rotShip.x << " " << rotShip.y << " " << rotShip.z << std::endl;

}


float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
	Input* input = Input::instance();

	if (input->isKeyPressed(GLFW_KEY_W)) {
		//camera.handleKeyboard(Camera::Movement::Forward, dt);
		ShipMovement(Movement::Forward, dt);
		ShipDirection(Movement::Forward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		camera.handleKeyboard(Camera::Movement::Backward, dt);
		ShipMovement(Movement::Backward, dt);
		ShipDirection(Movement::Backward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		//camera.handleKeyboard(Camera::Movement::Left, dt);
		ShipMovement(Movement::Left, dt);
		ShipDirection(Movement::Left, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		//camera.handleKeyboard(Camera::Movement::Right, dt);
		ShipMovement(Movement::Right, dt);
		ShipDirection(Movement::Right, dt);
	}
	
	if (input->isKeyPressed(GLFW_KEY_SPACE) && shoot==false) {
		shoot = true;
		posBullets.push_back(posShip);
		std::cout << "SHOOT" << posBullets.size() << std::endl;
	}
	else if (input->isKeyReleased(GLFW_KEY_SPACE) && shoot == true) {
		shoot = false;
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

bool in_frustum(glm::mat4 M, glm::vec3 p) {
	glm::vec4 Pclip = M * glm::vec4(p, 1.);
	return abs(Pclip.x) < Pclip.w &&
		abs(Pclip.y) < Pclip.w &&
		0 < Pclip.z &&
		Pclip.z < Pclip.w;
}

GLboolean CheckCollisionXZ(GameObject& go1, GameObject& go2) // AABB - AABB collision
{
	
	// Collision x-axis?
	bool collisionX = (go1.Position().x + go1.getSize().x >= go2.Position().x &&
		go2.Position().x + go2.getSize().x >= go1.Position().x) ||
		(go1.Position().x - go1.getSize().x <= go2.Position().x &&
			go2.Position().x - go2.getSize().x <= go1.Position().x);
	// Collision y-axis?
	bool collisionZ = (go1.Position().z + go1.getSize().z >= go2.Position().z &&
		go2.Position().z + go2.getSize().z>= go1.Position().z) || 
		(go1.Position().z - go1.getSize().z <= go2.Position().z &&
			go2.Position().z - go2.getSize().z <= go1.Position().z);
	// Collision only if on both axes
	

	//std::cout << collisionX << " " << collisionZ << std::endl;
	//std::cout << collisionX << " " << go1.Transform().getPosition().x << " " << go2.Transform().getPosition().x << std::endl;

	return collisionX && collisionZ;
}

/*
void render(const Geometry& floor, const Model& object, const Model& enemy, const Geometry& sphere, const Shader& s_phong, const Shader& s_normal, const Shader& s_light, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

	//std::cout << proj << std::endl;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat3 normalMat = glm::mat3(1.0f);

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

		//sphere.render();
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

		//sphere.render();
	}


	s_normal.use();

	s_normal.set("viewPos",camera.getPosition());

	//DIRLIGHT
	s_normal.set("Light.direction", dirLight.getDirection());
	s_normal.set("Light.ambient", dirLight.getAmbient());
	s_normal.set("Light.diffuse", dirLight.getDiffuse());
	s_normal.set("Light.specular", dirLight.getSpecular());
	s_normal.set("material.shininess", 128);

	//POINTLIGHT
	for (uint32_t i = 0; i < nPointLight; ++i) {
		pointLights[i].setShader(s_normal, i);
	}

	//SPOTLIGHT
	for (uint32_t i = 0; i < nSpotLight; ++i) {
		spotLights[i].setShader(s_normal, i);
	}

	//Plane/Floor

	GameObject GOfloor(glm::vec3(0.0f, -0.5f, 0.0f));
	GOfloor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	GOfloor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));

	GOfloor.Draw(s_normal, floor, view, proj, t_albedo, t_specular, t_normal);

	//OBJECT IMPORTED - SHIP

	GameObject ship(posShip);
	ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	ship.Draw(s_normal,object,view,proj,true);
	
	//glm::vec3 size = ship.Transform().getPosition()* view;

	//std::cout << size.x << std::endl;

	//OBJECT IMPORTED - SHIP

	GameObject enemyUFO(glm::vec3(1.0f, 2.0f, -2.0f));
	enemyUFO.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//enemyUFO.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	enemyUFO.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	enemyUFO.setSize(glm::vec3(0.34f));

	if(enemyAlive)
		enemyUFO.Draw(s_normal, enemy, view, proj, true);
	

	//BULLET
	if (posBullets.size()>0) {
		s_light.use();
		for (int i = 0; i < posBullets.size(); i++) {
			
			GameObject bullet(posBullets[i]);
			bullet.Scale(glm::vec3(0.25f));
			s_light.set("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
			bullet.Draw(s_light, sphere, view, proj, false);
			bullet.setSize(glm::vec3(0.1f));
			posBullets[i] += forwardBullet * 0.025f;

			bool Collision = CheckCollisionXZ(bullet, enemyUFO);

			if (Collision) enemyAlive = false;


			if(in_frustum(proj,posBullets[i]) || Collision){
				posBullets.erase(posBullets.begin() + i);
			}
		}
	}


}
*/

void render(SceneGraph& sceneGraph, GameObject& ship, GameObject& floor) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);
	floor.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
	floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	floor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));
	floor.readyToDraw();

	ship.Translate(posShip);
	ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	ship.readyToDraw();

	//sceneGraph.setViewProj(view, proj, camera);

	sceneGraph.updateNodes(view, proj, camera);
}



int main(int, char* []) {
	Window* window = Window::instance();
	window->setWidth(600);
	window->setHeight(800);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);


	const Shader s_phong("../projects/FINAL/phong.vs", "../projects/FINAL/blinn.fs");
	const Shader s_normal("../projects/FINAL/normalAll.vs", "../projects/FINAL/normalAll.fs");
	const Shader s_light("../projects/EJ10_01/light.vs", "../projects/EJ10_01/light.fs");
	const Model object("../assets/models/Sci_Fi_Fighter_Ship_v1/13897_Sci-Fi_Fighter_Ship_v1_l1.obj");
	const Model enemy("../assets/models/UFO/Low_poly_UFO.obj");

	const Texture t_albedo("../assets/textures/Lavabrick/Lavabrick_ILL.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/Lavabrick/lavabrick_TEX_DISP.jpg", Texture::Format::RGB);
	const Texture t_normal("../assets/textures/Lavabrick/lavabrick_TEX_NRM.jpg", Texture::Format::RGB);
	const Sphere sphere(0.1f, 50, 50);
	const Quad quad(1.0f);

	//Lights
	DirLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

	pointLights = {
		{ glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-3.0f, 2.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) }
	};
	spotLights = {
		{ glm::vec3(-1.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(1.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) }
	};



	
	//Load Assets
	Assets assets;
	assets.addNewGeometry(AssetsGeometry::quad,1.0, t_albedo, t_specular, t_normal,0);
	//assets.addNewGeometry(AssetsGeometry::sphere, 0.1f,1);
	assets.addNewModel(object, 2);
	//assets.addNewModel(enemy, 3);

	//std::cout << assets._models[0].assetID << std::endl;
	//std::cout << assets._models[0].model.directory_ << std::endl;
	

	//Create Materials with Shader
	//Material mainMaterial(Shader("../projects/FINAL/normalAll.vs", "../projects/FINAL/normalAll.fs"), dirLight, pointLights, spotLights);
	Material mainMaterial(s_normal, dirLight, spotLights, pointLights);
	//Material mainMaterial(Shader("../projects/FINAL/normalAll.vs", "../projects/FINAL/normalAll.fs"));

	//GObjects...

	//SceneGraph - Nodes <-Do it in GameObjects
	//std::vector<Model> models = { object, enemy };

	SceneGraph sceneGraph(assets);
	//sceneGraph.addNewNode(Node(2, mainMaterial,Node::Type::Model));
	//sceneGraph.addNewNode(Node(3, mainMaterial, Node::Type::Model));
	//sceneGraph.addNewNode(Node(0, mainMaterial, Node::Type::Geometry));
	
	//Add GameObjects
	GameObject ship(sceneGraph, Node(2, mainMaterial, Node::Type::Model));
	GameObject floor(sceneGraph, Node(0, mainMaterial, Node::Type::Geometry));

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
		//render(quad, object, enemy, sphere, s_phong, s_normal, s_light, t_albedo, t_specular, t_normal);
		render(sceneGraph, ship, floor);
		window->frame();
	}

	return 0;
}