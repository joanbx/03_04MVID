#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <chrono>
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


private:


	Ship& _player;
	std::vector<Bullet>& _bullets;
	std::vector<Enemy>& _enemies;
	std::vector<Asteroid>& _asteroids;
	bool _prevInScene = false;
	float _speed = 0.0025f;
	std::chrono::milliseconds  time_start;
	std::chrono::milliseconds  time_elapsed;
	std::uint32_t thresholdTimeShoot = 1000; //In ms


};

#endif