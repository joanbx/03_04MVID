#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <gameplay\ship.hpp>
#include <gameplay\enemy.hpp>
#include <engine\particleSystem.hpp>
#include <gameplay\asteroid.hpp>

class EnemyManager {

public:

	EnemyManager(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, std::vector<Asteroid>& asteroids, Ship& player);

	void Start();

	void Update(float dt);

	void UpdatePS(float dt);

	void setTimeStart(float time);

	float getTimeElapsed() { return _time_elapsed; }

private:

	Ship& _player;
	std::vector<Bullet>& _bullets;
	std::vector<Enemy>& _enemies;
	std::vector<Asteroid>& _asteroids;
	bool _prevInScene = false;
	float _initialSpeedEnemy = 0.5f;
	float _initalFrequencyShootEnemy = 3.0f;
	float _initialEnemyBulletSpeed = 0.5f;
	float  _time_start;
	float  _time_elapsed;
	int _maxEnemies = 1;
	int _maxAsteroids = 1;

};

#endif