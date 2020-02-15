#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <gameplay\ship.hpp>
#include <gameplay\enemy.hpp>
#include <engine\particleSystem.hpp>
#include <gameplay\asteroid.hpp>

class EnemyManager : public GameBehaviour {

public:

	EnemyManager(std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, Ship& player);
	~EnemyManager();

	void Start() final;

	void Update(const float dt) final;

	void UpdatePS(float dt);

	void setTimeStart(float time);

	uint32_t getWave() { return _wave; }
	float getTimeElapsed() { return _time_elapsed; }

private:

	Ship& _player;
	std::vector<Enemy>& _enemies;
	std::vector<Asteroid>& _asteroids;
	bool _prevInScene = false;
	float _initialSpeedEnemy = 0.4f;
	float _initalFrequencyShootEnemy = 3.4f;
	float _initialEnemyBulletSpeed = 0.6f;
	float  _time_start;
	float  _time_elapsed;
	uint32_t _maxEnemies = 1;
	uint32_t _maxAsteroids = 1;
	uint32_t _wave = 0;

};

#endif