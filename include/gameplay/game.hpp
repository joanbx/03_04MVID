#ifndef __GAME_H__
#define __GAME_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\floor.hpp>
#include <gameplay\bullet.hpp>
#include <gameplay\enemy.hpp>
#include <gameplay\asteroid.hpp>
#include <gameplay\ship.hpp>
#include <gameplay\enemyManager.hpp>
#include <engine\textRenderer.hpp>


//class Game: Shootem up Game
class Game {
public:
	//Constructor
	Game(uint32_t width = 600, uint32_t height= 800);
	~Game();

	//Start (Definition)
	void Start();
	//Update / Loop
	void Update(float dt, SceneGraph& sceneGraph, Ship& ship, Floor& floor, EnemyManager& enemyMng, TextRenderer& textRenderer);
	
	

private:

	uint32_t _width;
	uint32_t _height;
	bool _gameStarted = false;
	bool _isFirstFrameinGame = true;
	uint32_t _prevLifeShip;
	uint32_t _prevWave;
	bool _restartEnemies = false;
	float _timeShowWaveMsgInit;
	bool _drawWave = false;
	
	//Start game button
	void startButton();
};

#endif