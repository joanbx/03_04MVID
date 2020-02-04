/*
EJ10.01 - Importar un modelo 3D complejo a elección (de algún repositorio de modelos 3D gratis). El modelo tiene que tener lo necesario para pintarlo correctamente. Para pintar hay que usar el modelo de iluminación Blinn, mapas difusos y speculares, y usar al menos una luz direccional, y una point light.
Autor: Joan Baixauli
Ejercicio basado en: AG10
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <list>
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
#include "engine/easyGO.hpp"
#include "engine/drawable.hpp"
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
DirLight dirLight_(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));


PointLight* pointLights_ = new PointLight[nPointLight]{
	{glm::vec3(0.0f,2.0f,0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(3.0f,2.0f,2.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(-3.0f,2.0f,-2.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f,1.0f,1.0f)}

};

SpotLight* spotLights_ = new SpotLight[nSpotLight]{
	{glm::vec3(-1.0f,0.25f,0.0f), glm::vec3(0.0f, -1.0f,0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(1.0f,0.25f,0.0f),glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)},
	{glm::vec3(0.0f,4.25f,0.0f),glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f),glm::vec3(1.0f,1.0f,1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f,1.0f,1.0f)}
};

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

GLboolean CheckCollisionXZ(EasyGO& go1, EasyGO& go2) // AABB - AABB collision
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
	s_normal.set("Light.direction", dirLight_.getDirection());
	s_normal.set("Light.ambient", dirLight_.getAmbient());
	s_normal.set("Light.diffuse", dirLight_.getDiffuse());
	s_normal.set("Light.specular", dirLight_.getSpecular());
	s_normal.set("material.shininess", 128);

	//POINTLIGHT
	for (uint32_t i = 0; i < nPointLight; ++i) {
		pointLights_[i].setShader(s_normal, i);
	}

	//SPOTLIGHT
	for (uint32_t i = 0; i < nSpotLight; ++i) {
		spotLights_[i].setShader(s_normal, i);
	}

	//Plane/Floor

	EasyGO GOfloor(glm::vec3(0.0f, -0.5f, 0.0f));
	GOfloor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	GOfloor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));

	GOfloor.Draw(s_normal, floor, view, proj, t_albedo, t_specular, t_normal);

	//OBJECT IMPORTED - SHIP

	EasyGO ship(posShip);
	ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	ship.Draw(s_normal,object,view,proj,true);
	
	//glm::vec3 size = ship.Transform().getPosition()* view;

	//std::cout << size.x << std::endl;

	//OBJECT IMPORTED - SHIP

	EasyGO enemyUFO(glm::vec3(1.0f, 2.0f, -2.0f));
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
			
			EasyGO bullet(posBullets[i]);
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

void render(Assets& assets, EasyGO GOFloor, const Geometry& floor, const Shader& s_normal, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

	//std::cout << proj << std::endl;


	
	s_normal.use();

	s_normal.set("viewPos", camera.getPosition());

	//DIRLIGHT
	s_normal.set("Light.direction", dirLight_.getDirection());
	s_normal.set("Light.ambient", dirLight_.getAmbient());
	s_normal.set("Light.diffuse", dirLight_.getDiffuse());
	s_normal.set("Light.specular", dirLight_.getSpecular());
	s_normal.set("material.shininess", 128);

	//POINTLIGHT
	for (uint32_t i = 0; i < nPointLight; ++i) {
		pointLights_[i].setShader(s_normal, i);
	}

	//SPOTLIGHT
	for (uint32_t i = 0; i < nSpotLight; ++i) {
		spotLights_[i].setShader(s_normal, i);
	}

	//Plane/Floor

	GOFloor.Init();
	GOFloor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	GOFloor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));

	//std::cout << assets.getAssetGeometry(0)._textures.size() << std::endl;



	//std::cout << _textures.size() << std::endl;
	//GOFloor.Draw(s_normal, assets.getAssetGeometry(0).getGeometry(), view, proj, _textures[0], _textures[1], _textures[2]);
	GOFloor.Draw(s_normal, assets.getAssetGeometry(0).getGeometry(), view, proj, assets.getAssetGeometry(0).getAlbedo(), assets.getAssetGeometry(0).getSpecular(), assets.getAssetGeometry(0).getNormal());


	

	//material.setMaterialLights();


	//OBJECT IMPORTED - SHIP
	//EasyGO ship(posShip);

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, posShip);
	//model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(ship.go, 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//if (angleShip != 0.0f) glm::rotate(model, angleShip, rotShip);
	//model = glm::scale(ship.go, glm::vec3(0.001f, 0.001f, 0.001f));

	//ship.Init();
	//ship.Translate(posShip);
	//ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	//ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	////ship.Draw(s_normal, object, view, proj, true);
	//
	////const Model& m = assets.getModel(0);
	//ship.Draw(s_normal, assets.getModel(0), view, proj, true);

	//FLOOR

	/*floor.Init();
	floor.Translate(glm::vec3(10.0f, 0.0f, 0.0f));
	floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	floor.Scale(glm::vec3(5.0f, 5.0f, 5.0f));*/

	/*EasyGO GOfloor(glm::vec3(0.0f, -0.5f, 0.0f));
	GOfloor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	GOfloor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));

	GOfloor.Draw(s_normal, quadfloor, view, proj, t_albedo, t_specular, t_normal);*/


	//quadfloor.render();

	//GOfloor.Draw(s_normal, quadfloor, view, proj, t_albedo, t_specular, t_normal);

	//floor.Draw(s_normal, quadfloor, view, proj, t_albedo, t_specular, t_normal);

	//floor.Draw(s_normal, assets.getGeometry(0), view, proj, assets.getAssetGeometry(0)._textures[0], assets.getAssetGeometry(0)._textures[1], assets.getAssetGeometry(0)._textures[2]);
	//ship.Draw(s_normal, assets.getGeometry(0), view, proj, false);

	//Draw(model, s_normal, assets.getModel(0), view, proj, true);
	//assets.getModel(0).render(s_normal);
	
	//material._shader.use();

	//material._shader.set("viewPos", camera.getPosition());

	//material.setMaterialLights();


	////OBJECT IMPORTED - SHIP
	/*EasyGO ship(posShip);
	ship.Translate(posShip);
	ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	ship.Draw(material._shader, object, view, proj, true);*/



}


void render(SceneGraph& sceneGraph, GameObject& ship, GameObject& floor, std::vector<GameObject>& enemies) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);
	
	floor.Init();
	floor.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
	floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	floor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));
	floor.readyToDraw();

	ship.Init();
	ship.Translate(posShip);
	ship.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ship.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) ship.Rotate(angleShip, rotShip);
	ship.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	ship.readyToDraw();


	enemies[0].Init();
	enemies[0].Translate(glm::vec3(1.0f, 2.0f, -2.0f));
	enemies[0].Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	enemies[0].Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	enemies[0].setSize(glm::vec3(0.34f));
	enemies[0].readyToDraw();

	sceneGraph.updateNodes(view, proj, camera.getPosition());
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
	const Model enemy_("../assets/models/UFO/Low_poly_UFO.obj");

	const Texture t_albedoLava("../assets/textures/Lavabrick/Lavabrick_ILL.png", Texture::Format::RGB);
	const Texture t_specularLava("../assets/textures/Lavabrick/lavabrick_TEX_DISP.jpg", Texture::Format::RGB);
	const Texture t_normalLava("../assets/textures/Lavabrick/lavabrick_TEX_NRM.jpg", Texture::Format::RGB);
	const Sphere sphere(0.1f, 50, 50);
	const Quad quad(1.0f);

	//std::cout << object.directory_ << std::endl;

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
	int assetFloor = assets.addNewGeometry(quad, t_albedoLava, t_specularLava, t_normalLava);
	//assets.addNewGeometry(AssetsGeometry::sphere, 0.1f,1);
	int assetShip = assets.addNewModel(object);
	int assetEnemy = assets.addNewModel(enemy_);

	Material mainMaterial(s_normal, dirLight, spotLights, pointLights);
	//Material mainMaterial(Shader("../projects/FINAL/normalAll.vs", "../projects/FINAL/normalAll.fs"));

	//GObjects...

	//SceneGraph - Nodes <-Do it in GameObjects
	//std::vector<Model> models = { object, enemy };

	SceneGraph sceneGraph(assets);

	
	//Add GameObjects
	GameObject ship(sceneGraph,Node(assetShip, mainMaterial, Node::Type::isModel));
	GameObject floor(sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry));
	
	//Enemy Pool
	std::vector<GameObject> enemies = {
		GameObject(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel)),
		GameObject(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel)),
		GameObject(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel)),
		GameObject(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel))
	};

	EasyGO GoFloor(glm::vec3(0.0,0.0,0.0));

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
		//render(quad, object, enemy_, sphere, s_phong, s_normal, s_light, t_albedoLava, t_specularLava, t_normalLava);
		render(sceneGraph, ship, floor, enemies);
		//render(assets, GoFloor, quad, s_normal, t_albedo, t_specular, t_normal);
		window->frame();
	}

	return 0;
}