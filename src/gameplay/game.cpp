#include <gameplay/game.hpp>

Game::Game(uint32_t width, uint32_t height) : _width(width), _height(height)
{
	Start();
}

Game::~Game()
{
}

void Game::Start()
{
	
	//Define Window
	Window* window = Window::instance();
	window->setWidth(_width);
	window->setHeight(_height);
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

	//Define Shaders
	const Shader s_normal("../projects/FINAL/phongFinal.vs", "../projects/FINAL/blinnFinal.fs");
	const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
	const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
	const Shader s_text("../projects/FINAL/text.vs", "../projects/FINAL/text.fs");
	const Shader s_particle("../projects/FINAL/particle.vs", "../projects/FINAL/particle.fs");

	//Define models
	const Model object("../assets/models/ships/SF_Fighter/SciFi_Fighter.obj");
	const Model enemy01("../assets/models/ships/UFO/Low_poly_UFO.obj");
	const Model enemy02("../assets/models/ships/ship03/Spaceship.obj");
	const Model alienBullet("../assets/models/bullets/MetalAlien01/sphere.obj");
	const Model metalBullet("../assets/models/bullets/Metal_Pattern/sphere.obj");
	const Model shipBullet("../assets/models/bullets/Abstract_001/sphere.obj");
	const Model asteroid01("../assets/models/asteroid/asteroid.obj");

	//Define Geometries
	//const Sphere sphere(0.1f, 50, 50);
	const Cube cube(1.0f);
	const Quad quad(1.0f);
	const Quad quadTest(1.0f);

	//Define Textures
	const Texture t_albedoLava("../assets/textures/Lava_004_SD/Lava_004_COLOR.jpg", Texture::Format::RGB);
	const Texture t_specularLava("../assets/textures/Lava_004_SD/Lava_004_SPEC.png", Texture::Format::RGBA);
	const Texture t_normalLava("../assets/textures/Lava_004_SD/Lava_004_NORM.png", Texture::Format::RGBA);
	const Texture t_albedoPowerUp("../assets/textures/PowerUpBullet/Abstract_009_basecolor.jpg", Texture::Format::RGB);
	const Texture t_specularPowerUp("../assets/textures/PowerUpBullet/Abstract_009_roughness.jpg", Texture::Format::RGB);
	const Texture t_normalPowerUp("../assets/textures/PowerUpBullet/Abstract_009_normal.jpg", Texture::Format::RGB);
	const Texture t_particle("../assets/textures/explosion.png", Texture::Format::RGBA);


	// Define Lights
	DirLight dirLight(glm::vec3(1.2f, 5.0f, -1.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

	std::vector<PointLight> pointLights = {
		{ glm::vec3(3.0f, 5.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-3.0f, 5.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::vec3(1.0f, 1.0f, 1.0f) },
	};
	std::vector<SpotLight> spotLights = {
		{ glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.2f, 0.32f, 50.0, 60.0, glm::vec3(1.0f, 1.0f, 1.0f) },
	};

	//Define shadow
	Shadow shadow(s_depth, s_debug, quadTest, dirLight.getDirection());

	//Define Material (main shader with shadow and light properties custumized for our normal shader)
	Material mainMaterial(s_normal, shadow, dirLight, spotLights, pointLights);
	
	//Load Assets (container of all Models and Geometries along its textures used in the game)
	Assets assets;
	//int assetFloor = assets.addNewGeometry(quad, t_albedo, t_specular, t_normal);
	int assetFloor = assets.addNewGeometry(quad, t_albedoLava, t_specularLava, t_normalLava);
	int assetShip = assets.addNewModel(object); 
	int assetEnemy01 = assets.addNewModel(enemy01);
	int assetEnemy02 = assets.addNewModel(enemy02);
	int assetAsteroid01 = assets.addNewModel(asteroid01);
	int assetBulletType01 = assets.addNewModel(shipBullet);
	int assetBulletType02 = assets.addNewModel(alienBullet);
	int assetBulletType03 = assets.addNewModel(metalBullet);
	int assetpowerUp = assets.addNewGeometry(cube, t_albedoPowerUp, t_specularPowerUp, t_normalPowerUp);
	
	//Main Camera
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
		{sceneGraph, Node(assetFloor, mainMaterial, Node::Type::isGeometry)}
	};
	//Floor manager 
	Floor floorMng(floors);


	//Bullets for the Enemy
	std::vector<Bullet> bullets01Enemy = {
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType02, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy)
	};
	std::vector<Bullet> bullets02Enemy = {
		Bullet(sceneGraph, Node(assetBulletType03, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType03, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType03, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy),
		Bullet(sceneGraph, Node(assetBulletType03, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isEnemy)
	};
	//Bullet Player Pool
	std::vector<Bullet> bullets01Player = {
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer),
		Bullet(sceneGraph, Node(assetBulletType01, mainMaterial, Node::Type::isModel),Bullet::Bullettypes::isPlayer)
	};

	//Enemy pool
	std::vector<Enemy> enemies = {
		Enemy(sceneGraph, Node(assetEnemy01, mainMaterial, Node::Type::isModel), bullets01Enemy, ps, Enemy::EnemyTpye::EnemyVersion01),
		Enemy(sceneGraph, Node(assetEnemy02, mainMaterial, Node::Type::isModel), bullets02Enemy, ps, Enemy::EnemyTpye::EnemyVersion02),
		Enemy(sceneGraph, Node(assetEnemy01, mainMaterial, Node::Type::isModel), bullets01Enemy, ps, Enemy::EnemyTpye::EnemyVersion01),	
		Enemy(sceneGraph, Node(assetEnemy02, mainMaterial, Node::Type::isModel), bullets02Enemy, ps, Enemy::EnemyTpye::EnemyVersion02)
	};

	//Asteroid pool
	std::vector<Asteroid> asteroids = {
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)},
		{sceneGraph, Node(assetAsteroid01, mainMaterial, Node::Type::isModel)}
	};

	//Ship = Player
	Ship ship(sceneGraph, Node(assetShip, mainMaterial, Node::Type::isModel), bullets01Player, enemies) ;

	//Enemy Manager
	EnemyManager enemyMng(enemies, asteroids, ship);

	//PowerUp
	PowerUp powerUp(sceneGraph, Node(assetpowerUp, mainMaterial, Node::Type::isGeometry));

	//Loop - Update
	float lastFrame = 0.0f;
	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Update(deltaTime, sceneGraph, ship, floorMng, enemyMng, textRenderer, powerUp);
		window->frame();
	}
}

void Game::Update(const float dt, SceneGraph& sceneGraph, Ship& ship, Floor& floor, EnemyManager& enemyMng, TextRenderer& textRenderer, PowerUp& powerUp)
{
	//Scene graph update nodes
	sceneGraph.updateNodes();

	if (_gameStarted) {

		//First time
		if (_isFirstFrameinGame) {
			enemyMng.Start();
			ship.Start();
			enemyMng.setTimeStart(glfwGetTime());
			powerUp.Start();
			_prevLifeShip = ship.getLife();
			_isFirstFrameinGame = false;
		}
		//Restart enemeies
		else if (_restartEnemies) {
			enemyMng.Start();
			ship.setPowerUp(false);
			_restartEnemies = false;
		}

		//Updates:

		floor.Update(dt);
		
		ship.Update(dt);

		enemyMng.Update(dt);

		enemyMng.UpdatePS(dt);

		powerUp.Update(dt);
		
		//ship life to check if an enemy restart is needed
		if (_prevLifeShip != ship.getLife()) {
			_restartEnemies = true;
		}
		_prevLifeShip = ship.getLife();
		//Restart game
		if (ship.getLife() == 0) { 
			_isFirstFrameinGame = true;
			_gameStarted = false; 
		}

		//Wave
		if (_prevWave != enemyMng.getWave()) {
			_timeShowWaveMsgInit = glfwGetTime();
		}
		if (glfwGetTime() - _timeShowWaveMsgInit < 3) {
			_drawWave = true;
		}
		else {
			_drawWave = false;
		}
		_prevWave = enemyMng.getWave();
		
		//PowerUp
		if (powerUp.getUsed() && powerUp.getGO().CheckCollisionXZ(ship.getGO())) {
			ship.setPowerUp(true);
			powerUp.setUsed(false);
		}

		if (enemyMng.getWave()>1 && (int)enemyMng.getTimeElapsed() % 30 == 0) {
			powerUp.Start();
			powerUp.setUsed(true);
		}

		

		//Text renderer (Add) - HUD

		textRenderer.addTextToRender("Life: " + std::to_string(ship.getLife()), 5, 5, 1.0, glm::vec3(1.0, 1.0, 1.0)); //Text: life
		textRenderer.addTextToRender("Points: " + std::to_string(ship.getKills() + (enemyMng.getWave() - 1) * 50), _width - 200, 5, 1.0, glm::vec3(1.0, 1.0, 1.0)); //Text: Points
		textRenderer.addTextToRender("Time: " + convertTimeToStringmmss(enemyMng.getTimeElapsed()), 5, _height - 25, 1.0, glm::vec3(1.0, 1.0, 1.0)); //Text: Time
		if (_drawWave) textRenderer.addTextToRender("Wave " + std::to_string(enemyMng.getWave()), _width / 2 - 50.0, _height / 2, 1.0, glm::vec3(1.0, 0.0, 0.0)); //Draw new Wave
	}
	
	//Start menu
	else {
		textRenderer.addTextToRender("Shoot Em Up 03_04MVID", _width / 2 - 130.0, _height / 2 -100, 1.0, glm::vec3(1.0, 1.0, 1.0));

		if ((int)glfwGetTime() % 2 == 1) {
			textRenderer.addTextToRender("PRESS X TO START ", _width / 2 - 110.0, _height / 2, 1.0, glm::vec3(1.0, 1.0, 1.0));
		}
		textRenderer.addTextToRender("Controls: WASD + SPACEBAR", _width / 2 - 148.0, _height / 2 + 100, 1.0, glm::vec3(1.0, 1.0, 1.0));
		startButton();
	}
	
	//Render text
	textRenderer.RenderText();
	
}

void Game::startButton()
{
	Input* input = Input::instance();
	if (input->isKeyPressed(GLFW_KEY_X) && !_gameStarted) {
		_gameStarted = true;
	}
}

std::string Game::convertTimeToStringmmss(float time) {
	uint32_t minutes = (int)time / 60;
	uint32_t seconds = (int)time % 60;
	std::string strMinutes;
	std::string strSeconds;
	if (minutes < 10) strMinutes = "0" + std::to_string(minutes);
	else strMinutes = std::to_string(minutes);
	if (seconds<10) strSeconds = "0" + std::to_string(seconds);
	else strSeconds = std::to_string(seconds);

	return strMinutes + ":" + strSeconds;
}