/*
EJ10.01 - Importar un modelo 3D complejo a elección (de algún repositorio de modelos 3D gratis). El modelo tiene que tener lo necesario para pintarlo correctamente. Para pintar hay que usar el modelo de iluminación Blinn, mapas difusos y speculares, y usar al menos una luz direccional, y una point light.
Autor: Joan Baixauli
Ejercicio basado en: AG10
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
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
#include "engine/assets.hpp"
#include "engine/node.hpp"
#include <engine\sceneGraph.hpp>
#include <engine\shadow.hpp>
#include <gameplay\ship.hpp>
#include <gameplay\bullet.hpp>
#include <gameplay\enemy.hpp>
#include <time.h>
#include <gameplay\enemyManager.hpp>
#include <engine\textRenderer.hpp>
#include <engine\particleSystem.hpp>

Camera camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0, -90);
glm::vec3 posShip(0.0f, 2.0f, 0.0f);
glm::vec3 rotShip(0.0f, 0.0f, 0.0f);
float angleShip = 0.0f;
uint8_t nPointLight = 3;
uint8_t nSpotLight = 3;
const uint32_t k_shadow_height = 1024;
const uint32_t k_shadow_width = 1024;
const float k_shadow_near = 1.0f;
const float k_shadow_far = 7.5f;

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


//SpotLight* spotLights;




float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
	Input* input = Input::instance();

	if (input->isKeyPressed(GLFW_KEY_W)) {
		//camera.handleKeyboard(Camera::Movement::Forward, dt);
		//ShipMovement(Movement::Forward, dt);
		//ShipDirection(Movement::Forward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		//camera.handleKeyboard(Camera::Movement::Backward, dt);
		//ShipMovement(Movement::Backward, dt);
		///ShipDirection(Movement::Backward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		//camera.handleKeyboard(Camera::Movement::Left, dt);
		//ShipMovement(Movement::Left, dt);
		//ShipDirection(Movement::Left, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		//camera.handleKeyboard(Camera::Movement::Right, dt);
		//ShipMovement(Movement::Right, dt);
		//ShipDirection(Movement::Right, dt);
	}
	
	//if (input->isKeyPressed(GLFW_KEY_SPACE) && shoot==false) {
		//shoot = true;
		//posBullets.push_back(posShip);
		//std::cout << "SHOOT" << posBullets.size() << std::endl;
	//}
	//else if (input->isKeyReleased(GLFW_KEY_SPACE) && shoot == true) {
		//shoot = false;
	//}
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

	//camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
	camera.handleMouseScroll(y);
}




void render(SceneGraph& sceneGraph, float dt, Ship& ship, GameObject& floor, EnemyManager& enemyMng, TextRenderer& textRenderer, ParticleSystem& ps) {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	//glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);
	
	floor.Init();
	floor.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
	floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	floor.Scale(glm::vec3(15.0f, 15.0f, 15.0f));
	floor.readyToDraw();


	
	ship.Update(dt);
	enemyMng.Update(dt);
	
	sceneGraph.updateNodes();

	enemyMng.UpdatePS(dt);

	//ps.Update(dt, 2, glm::vec2(0.25f,0));

	//ps.Draw();

	//textRenderer.RenderText("TEST!!!", 300.0, 800 / 2 - 20.0, 2.0, glm::vec3(0.0, 0.0, 1.0));

}



int main(int, char* []) {
	Window* window = Window::instance();
	window->setWidth(600);
	window->setHeight(800);

	float top = 12.0f * tan((camera.getFOV() * 0.5f) * 3.14f / 180.0f);
	float r = top * (float)600 / (float)800;

	std::cout << top << " " << r << std::endl;

	srand(time(NULL));

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);


	//const Shader s_phong("../projects/FINAL/phong.vs", "../projects/FINAL/blinn.fs");
	const Shader s_normal("../projects/TEST/normalTest.vs", "../projects/TEST/normalTest.fs"); //"../projects/FINAL/normalAll.vs", "../projects/FINAL/normalAll.fs"
	const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
	const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
	const Shader s_text("../projects/FINAL/text.vs", "../projects/FINAL/text.fs");
	const Shader s_particle("../projects/FINAL/particle.vs", "../projects/FINAL/particle.fs");

	//const Shader s_light("../projects/EJ10_01/light.vs", "../projects/EJ10_01/light.fs");
	const Model object("../assets/models/Freighter/Freigther_BI_Export.obj"); //Sci_Fi_Fighter_Ship_v1/13897_Sci-Fi_Fighter_Ship_v1_l1.obj //Freighter/Freigther_BI_Export.obj
	const Model enemy_("../assets/models/UFO/Low_poly_UFO.obj");
	const Model msphere("../assets/models/geometries/Sphere01/sphere.fbx");
	//const Model enemy2_("../assets/models/UFO/Low_poly_UFO.obj");
	const Texture t_albedoLava("../assets/textures/Lavabrick/Lavabrick_ILL.png", Texture::Format::RGB);
	const Texture t_specularLava("../assets/textures/Lavabrick/lavabrick_TEX_DISP.jpg", Texture::Format::RGB);
	const Texture t_normalLava("../assets/textures/Lavabrick/lavabrick_TEX_NRM.jpg", Texture::Format::RGB);
	
	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	const Texture t_normal("../assets/textures/bricks_normal.png", Texture::Format::RGB);

	const Texture t_particle("../assets/textures/explosion.png", Texture::Format::RGBA);

	const Sphere sphere(0.1f, 50, 50);
	const Cube cube(1.0f);
	const Quad quad(1.0f);
	const Quad quadTest(1.0f);

	//std::cout << msphere.directory_ << std::endl;

	s_particle.set("proj", camera.getProj());
	s_particle.set("view", camera.getViewMatrix());
	
	ParticleSystem ps(s_particle,t_particle,quadTest,30);
	//s_particle.set("sprite", 0);
	

	TextRenderer textRenderer(s_text,600,800);
	textRenderer.Load("../assets/fonts/Blanka-Regular.ttf",12);
	
	//Lights
	DirLight dirLight(glm::vec3(1.2f, 5.0f, -1.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
	//DirLight dirLight;

	std::vector<PointLight> pointLights = {
		{ glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
		//{ glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
		//{ glm::vec3(-3.0f, 2.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) }
	};
	std::vector<SpotLight> spotLights = {
		{ glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
		//{ glm::vec3(1.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
		//{ glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) }
	};


	Shadow shadow(s_depth, s_debug, quadTest, dirLight.getDirection());
	
	//Load Assets
	Assets assets;
	int assetFloor = assets.addNewGeometry(quad, t_albedo, t_specular, t_normal);
	//assets.addNewGeometry(AssetsGeometry::sphere, 0.1f,1);
	int assetShip = assets.addNewModel(object);  //assets.addNewModel(object); //assets.addNewGeometry(cube, t_albedo, t_specular, t_normal); 
	int assetEnemy = assets.addNewModel(enemy_);
	//int assetEnemy2 = assets.addNewModel(enemy2_);
	int assetBulletType01 = assets.addNewGeometry(cube, t_albedo, t_specular, t_normal);
	int assetBulletType02 = assets.addNewModel(msphere);
	Material mainMaterial(s_normal, shadow, dirLight, spotLights, pointLights);


	SceneGraph sceneGraph(assets, camera);

	
	//Add GameObjects
	//const GameObject shipgo(sceneGraph, Node(assetShip, mainMaterial, Node::Type::isModel));
	

	GameObject floor(sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry));
	
	//Bullet Player Pool
	std::vector<Bullet> bullets01Enemy = {
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy)
	};

	//Enemy Pool
	std::vector<Enemy> enemies = {
		Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy, ps),
		Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy, ps)
		//Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy),
		//Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy)
	};
	enemies[0].setInScene(true);
	//Bullet Player Pool
	std::vector<Bullet> bullets01Player = {
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isGeometry),Bullet::Bullettypes::isPlayer)
	};

	//EasyGO GoFloor(glm::vec3(0.0,0.0,0.0));

	//Ship = Player
	Ship ship(sceneGraph, Node(assetShip, mainMaterial, Node::Type::isModel), bullets01Player, enemies);

	EnemyManager enemyMng(enemies, bullets01Enemy, ship);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	Input::instance()->setKeyPressedCallback(onKeyPress);
	Input::instance()->setMouseMoveCallback(onMouseMoved);
	Input::instance()->setScrollMoveCallback(onScrollMoved);



	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(deltaTime);
		//render(quad, object, enemy_, sphere, s_phong, s_normal, s_light, t_albedoLava, t_specularLava, t_normalLava,fbo.first, fbo.second);
		render(sceneGraph, deltaTime, ship, floor, enemyMng, textRenderer, ps);
		//render(assets, GoFloor, quad, s_normal, t_albedo, t_specular, t_normal);
		window->frame();
	}

	return 0;
}