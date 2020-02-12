#include "..\..\include\gameplay\game.hpp"


Game::Game(uint32_t width, uint32_t height) : _width(width), _height(height)
{
	Start();
}

void Game::Start()
{
	
	//Define Window
	Window* window = Window::instance();
	window->setWidth(_width);
	window->setHeight(_height);
	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	//Define Shaders
	const Shader s_normal("../projects/FINAL/phongFinal.vs", "../projects/FINAL/blinnFinal.fs");
	const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
	const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
	const Shader s_text("../projects/FINAL/text.vs", "../projects/FINAL/text.fs");
	const Shader s_particle("../projects/FINAL/particle.vs", "../projects/FINAL/particle.fs");

	//Define models
	const Model object("../assets/models/ships/SF_Fighter/SciFi_Fighter.obj");
	const Model enemy_("../assets/models/UFO/Low_poly_UFO.obj");
	const Model msphere("../assets/models/bullets/sphere.obj");
	const Model asteroid01("../assets/models/asteroid/asteroid.obj");

	//Define Geometries
	const Sphere sphere(0.1f, 50, 50);
	const Cube cube(1.0f);
	const Quad quad(1.0f);
	const Quad quadTest(1.0f);

	//Define Textures
	const Texture t_albedoLava("../assets/textures/Lavabrick/Lavabrick_ILL.png", Texture::Format::RGB);
	const Texture t_specularLava("../assets/textures/Lavabrick/lavabrick_TEX_DISP.jpg", Texture::Format::RGB);
	const Texture t_normalLava("../assets/textures/Lavabrick/lavabrick_TEX_NRM.jpg", Texture::Format::RGB);
	const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	const Texture t_normal("../assets/textures/bricks_normal.png", Texture::Format::RGB);
	const Texture t_particle("../assets/textures/explosion.png", Texture::Format::RGBA);

	// Define Lights
	DirLight dirLight(glm::vec3(1.2f, 5.0f, -1.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

	std::vector<PointLight> pointLights = {
		{ glm::vec3(0.0f, 4.5f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
	};
	std::vector<SpotLight> spotLights = {
		{ glm::vec3(0.0f, 4.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 30.0, 40.0, glm::vec3(1.0f, 1.0f, 1.0f) },
	};

	//Define shadow
	Shadow shadow(s_depth, s_debug, quadTest, dirLight.getDirection());

	//Define Material (main shader with shadow and light properties)
	Material mainMaterial(s_normal, shadow, dirLight, spotLights, pointLights);

	//Load Assets (container of Models and Geometries along its texture)
	Assets assets;
	int assetFloor = assets.addNewGeometry(quad, t_albedo, t_specular, t_normal);
	int assetFloor2 = assets.addNewGeometry(quad, t_albedoLava, t_specularLava, t_normalLava);
	int assetShip = assets.addNewModel(object); 
	int assetEnemy = assets.addNewModel(enemy_);
	int assetAsteroid01 = assets.addNewModel(asteroid01);
	int assetBulletType01 = assets.addNewGeometry(cube, t_albedo, t_specular, t_normal);
	int assetBulletType02 = assets.addNewModel(msphere);
	
	//Camera
	Camera camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0, -90);

	//Scene graph with the assets to be used and the main camera
	SceneGraph sceneGraph(assets, camera);
	
	//Particle system
	ParticleSystem ps(s_particle, t_particle, quadTest, 6);

	//Text renderer
	TextRenderer textRenderer(s_text, 600, 800);
	textRenderer.Load("../assets/fonts/arial.ttf", 24);

	//Now we will define the different content (and different nodes that hangs from the scene graph)

	//Pool of GameObjects with the asset of the floor
	std::vector<GameObject> floors = {
		{sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry)},
		{sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry)},
		{ sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry) }
	};
	//Floor manager 
	Floor floorMng(floors);


	//Bullet Player Pool
	//Bullets for the Enemy
	std::vector<Bullet> bullets01Enemy = {
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy)
	};
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

	//Enemy pool
	std::vector<Enemy> enemies = {
		Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy, ps),
		Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy, ps)
		//Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy),
		//Enemy(sceneGraph, Node(assetEnemy, mainMaterial, Node::Type::isModel), bullets01Enemy)
	};

	//Asteroid pool
	std::vector<Asteroid> asteroids = {
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)}
	};

	//Ship = Player
	Ship ship(sceneGraph, Node(assetShip, mainMaterial, Node::Type::isModel), bullets01Player, enemies) ;

	//Enemy Manager
	EnemyManager enemyMng(enemies, bullets01Enemy, asteroids, ship);

	//Loop - Update
	float lastFrame = 0.0f;
	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Update(deltaTime, sceneGraph, ship, floorMng, enemyMng, textRenderer);
		window->frame();
	}
}

void Game::Update(float dt, SceneGraph& sceneGraph, Ship& ship, Floor& floor, EnemyManager& enemyMng, TextRenderer& textRenderer)
{
	sceneGraph.updateNodes();
	
	if (_gameStarted) {
		floor.Update(dt);
		
		ship.Update(dt);

		enemyMng.Update(dt);

		enemyMng.UpdatePS(dt);

	}

	
	else {
		textRenderer.addTextToRender("Shoot Em Up 03_04MVID", _width / 2 - 130.0, _height / 2 -100, 1.0, glm::vec3(1.0, 1.0, 1.0));
		bool blinker = (int)glfwGetTime() % 2 == 1;
		std::string test = blinker ? "true" : "false";
		std::cout << blinker << std::endl;

		if (!blinker) {
			textRenderer.addTextToRender("PRESS X TO START ", _width / 2 - 110.0, _height / 2, 1.0, glm::vec3(1.0, 1.0, 1.0));
		}

		startButton();
	}

	textRenderer.RenderText();
	
}

void Game::startButton()
{
	Input* input = Input::instance();
	if (input->isKeyPressed(GLFW_KEY_X) && !_gameStarted) {
		_gameStarted = true;
	}
}
