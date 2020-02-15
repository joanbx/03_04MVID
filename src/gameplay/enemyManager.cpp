#include "..\..\include\gameplay\enemyManager.hpp"

EnemyManager::EnemyManager(std::vector<Enemy>& enemies, std::vector<Asteroid>& asteroids, Ship& player) : _enemies(enemies), _asteroids(asteroids), _player(player)
{
	Start();
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Start()
{
	_time_elapsed = 0;
	//Start enemies and its bullets
	for (auto& enemy : _enemies) {
		enemy.Start();
		for (auto& bullet : enemy.getBullets()) {
			if (!bullet.getStarted()) {
				bullet.Start();
				bullet.setStarted(true);
			}
		}
	}
	for (auto& asteroid : _asteroids) {
		asteroid.Start();
	}

	//Reset start flag from bullets
	for (auto& enemy : _enemies) {
		for (auto& bullet : enemy.getBullets()) {
			if (bullet.getStarted()) {
				bullet.setStarted(false);
			}
		}
	}
	
}

void EnemyManager::Update(const float dt)
{
	_time_elapsed = (int)(glfwGetTime() - _time_start);
	float frequency_shoot_enemy = _initalFrequencyShootEnemy;
	float speedEnemy = _initialSpeedEnemy;
	float bulletSpeed = _initialEnemyBulletSpeed;

	if (_time_elapsed < 30) { //Wave1
		_wave = 1;
		_maxEnemies = 1;
		_maxAsteroids = 1;
	}
	else if (_time_elapsed < 60) { //Wave2
		_wave = 2;
		_maxEnemies = 2;
		_maxAsteroids = 1;
		frequency_shoot_enemy *= 0.9f;
	}
	else if (_time_elapsed < 120) { //Wave3
		_wave = 3;
		_maxEnemies = 2;
		_maxAsteroids = 2;
		frequency_shoot_enemy *= 0.8f;
		speedEnemy *= 1.1f;
		bulletSpeed *= 1.1f;
	}
	else if (_time_elapsed < 150) { //Wave4
		_wave = 4;
		_maxEnemies = 2;
		_maxAsteroids = 3;
		frequency_shoot_enemy *= 0.7f;
		speedEnemy *= 1.2f;
		bulletSpeed *= 1.1f;
	}
	else if (_time_elapsed < 180) { //Wave5
		_wave = 5;
		_maxEnemies = 3;
		_maxAsteroids = 4;
		frequency_shoot_enemy *= 0.7f;
		speedEnemy *= 1.2f;
		bulletSpeed *= 1.2;
	}else if (_time_elapsed < 210) { //Wave6
		_wave = 6;
		_maxEnemies = 3;
		_maxAsteroids = 5;
		frequency_shoot_enemy *= 0.7f;
		speedEnemy *= 1.3f;
		bulletSpeed *= 1.3;
	}
	else if (_time_elapsed < 240) { //Wave7
		_wave = 7;
		_maxEnemies = 4;
		_maxAsteroids = 5;
		frequency_shoot_enemy *= 0.7f;
		speedEnemy *= 1.3f;
		bulletSpeed *= 1.3;
	}else if (_time_elapsed < 270) { //Wave8
		_wave = 8;
		_maxEnemies = 4;
		_maxAsteroids = 6;
		frequency_shoot_enemy *= 0.6f;
		speedEnemy *= 1.4f;
		bulletSpeed *= 1.4;
	}

	uint32_t enemycount = 0;
	for (auto& enemy : _enemies) {		
		if (enemycount < _maxEnemies ) {
			if (enemy.getDestroy() == false) {
				enemy.setFrequencyShoot(frequency_shoot_enemy);
				enemy.setSpeed(speedEnemy);
				enemy.setSpeedBullet(bulletSpeed);
				enemy.setInScene(true);


				//Avoid collisions:
				uint32_t enemy2count = 0;
				bool continueStoped = false;
				for (auto& enemy2 : _enemies) {
					if (enemy2.getInScene()) {
						if (enemy2count != enemycount) {
							if (enemy.getChangeDirection() == false && enemy.getGO().CheckCollisionXZ(enemy2.getGO(), glm::vec3(0.1f, 0, 0.1f))) {
								enemy2.setChangeDirection(true);
							}
							else if (enemy.getChangeDirection() == true && enemy.getGO().CheckCollisionXZ(enemy2.getGO(), glm::vec3(0.1f, 0, 0.1f))) {
								continueStoped = true;
							}
						}
						enemy2count++;
					}
				}

				//Update enemy
				enemy.setPlayerPos(_player.getPosition());
				enemy.Update(dt);

				if (enemy.getChangeDirection() && continueStoped == false) {
					enemy.setChangeDirection(false);
				}
			}

			//Bullets: For each bullet, check wich one has been used and updated. If it has not been updated->update bullet, then check collision with player
			for (auto& bullet : enemy.getBullets()) {
				if (!bullet.getUpdated() && bullet.getUsed()) {
					bullet.setUpdated(true);
					bullet.Update(dt);
					if (bullet.getUsed() && bullet.getGO().CheckCollisionXZ(_player.getGO())) {
						_player.setDestroy();
						bullet.setUse(false);
					}
				}
			}
			
		}
		else {
			enemy.setInScene(false);
		}
		enemycount++;
		
	}

	//Reset update flag bullets
	for (auto& enemy : _enemies) {
		for (auto& bullet : enemy.getBullets()) {
			if (bullet.getUpdated()) {
				bullet.setUpdated(false);
			}
		}
	}
	
	//Asteroids
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
						//Restart asteroid
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

