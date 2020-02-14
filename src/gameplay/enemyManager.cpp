#include "..\..\include\gameplay\enemyManager.hpp"

EnemyManager::EnemyManager(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, std::vector<Asteroid>& asteroids, Ship& player) : _enemies(enemies), _bullets(bullets), _asteroids(asteroids), _player(player)
{
	Start();
}

void EnemyManager::Start()
{
	for (auto& enemy : _enemies) {
		enemy.Start();
	}
	for (auto& asteroid : _asteroids) {
		asteroid.Start();
	}
	for (auto& bullet : _bullets) {
		bullet.Start();
	}
}

void EnemyManager::Update(float dt)
{
	//std::cout << _enemies.size() << std::endl;
	_time_elapsed = glfwGetTime() - _time_start;
	float frequency_shoot_enemy = _initalFrequencyShootEnemy;
	float speedEnemy = _initialSpeedEnemy;
	float bulletSpeed = _initialEnemyBulletSpeed;

	if (_time_elapsed < 60) { //Wave1
		_maxEnemies = 2;
		_maxAsteroids = 1;
	}
	else if (_time_elapsed < 120) { //Wave2
		_maxEnemies = 3;
		_maxAsteroids = 2;
		frequency_shoot_enemy *= 0.8f;
		speedEnemy *= 1.2f;
		bulletSpeed *= 1.2;
		
	}
	else if (_time_elapsed < 180) { //Wave3
		_maxEnemies = 3;
		_maxAsteroids = 4;
		frequency_shoot_enemy *= 0.6f;
		speedEnemy *= 1.4f;
		bulletSpeed *= 1.4;
	}
	else if (_time_elapsed < 240) { //Wave4
		_maxEnemies = 4;
		_maxAsteroids = 8;
		frequency_shoot_enemy *= 0.5f;
		speedEnemy *= 1.5f;
		bulletSpeed *= 1.5;
	}

	uint32_t enemycount = 0;
	for (auto& enemy : _enemies) {		
		if (enemycount < _maxEnemies) {
			enemy.setFrequencyShoot(frequency_shoot_enemy);
			enemy.setSpeed(speedEnemy);
			enemy.setSpeedBullet(bulletSpeed);
			
			//Avoid collisions:
			uint32_t enemy2count = 0;
			bool continueStoped = false;
			for (auto& enemy2 : _enemies) {
				if (enemy2.getInScene()) {
					if (enemy2count != enemycount) {
						if (enemy.getChangeDirection() == false && enemy.getGO().CheckCollisionXZ(enemy2.getGO(), glm::vec3(1.0f, 0, 1.0f))) {
							enemy2.setChangeDirection(true);
						}
						else if (enemy.getChangeDirection() == true && enemy.getGO().CheckCollisionXZ(enemy2.getGO(), glm::vec3(1.0f, 0, 1.0f))) {
							continueStoped = true;
						}
					}
					enemy2count++;
				}	
			}

			/*for (auto& asteroid : _asteroids) {
				if (asteroid.getUsed()) {
					if (enemy.getChangeDirection() == false && enemy.getGO().CheckCollisionXZ(asteroid.getGO(), glm::vec3(0.5f, 0, 0.50f))) {
						enemy.setChangeDirection(true);
					}
					else if (enemy.getChangeDirection() == true && enemy.getGO().CheckCollisionXZ(asteroid.getGO(), glm::vec3(0.5f, 0, 0.5f))) {
						continueStoped = true;
					}

				}
				
			}*/

			if (enemy.getChangeDirection() && continueStoped == false) {
				enemy.setChangeDirection(false);
			}

			enemy.Update(dt, _player.getPosition());
		}
		
		
		enemycount++;
	}
	for (auto& bullet : _bullets) {
		bullet.Update(dt);
		if (bullet.getUsed() && bullet.getGO().CheckCollisionXZ(_player.getGO())) {
			_player.setDestroy();
			bullet.setUse(false);
		}
	}
	uint32_t asteroidCount = 0;
	for (auto& asteroid : _asteroids) {
		if (asteroidCount < _maxAsteroids) {
			asteroid.setSpeed(speedEnemy);
			asteroid.Update(dt);
			//Check collision player
			if (asteroid.getGO().CheckCollisionXZ(_player.getGO())) {
				_player.setDestroy();
				asteroid.setUse(false);
			} 

			//Check collision other enemies
			for (auto& enemy : _enemies) {
				if (enemy.getInScene()) {
					if (asteroid.getGO().CheckCollisionXZ(enemy.getGO())) {
						//enemy.setDestroy(true);
						//enemy.setInScene(false);
						asteroid.setUse(false);
						asteroid.Start();
					}
				}
			}
		}
		asteroidCount++;
		
	}
}

void EnemyManager::UpdatePS(float dt)
{
	for (auto& enemy : _enemies) {
		if (enemy.getDestroy() == true && !enemy.getPS().getFinished()) {
			enemy.getPS().Update(dt, 12, enemy.getGO(), glm::vec3(0.0f, 0.0f, 0.5f));
		}
		else if (enemy.getDestroy() == true && enemy.getPS().getFinished()) {
			std::cout << "START ENEMY " << std::endl;
			enemy.setInScene(true);
			enemy.Start();
		}
	}
}

void EnemyManager::setTimeStart(float time)
{
	_time_start = time;
}

