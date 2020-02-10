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
}

void EnemyManager::Update(float dt)
{
	//std::cout << _enemies.size() << std::endl;
	for (auto& enemy : _enemies) {
		enemy.Update(dt, _player.getPosition());
	}
	for (auto& bullet : _bullets) {
		bullet.Update(dt);
	}
	for (auto& asteroid : _asteroids) {
		asteroid.Update(dt);
	}
}

void EnemyManager::UpdatePS(float dt)
{
	for (auto& enemy : _enemies) {
		if (enemy.getDestroy() == true && !enemy.getPS().getFinished()) {
			enemy.getPS().Update(dt, 3, enemy.getGO(), glm::vec3(0.0f, 0.0f, 0.5f));
		}
		else if (enemy.getDestroy() == true && enemy.getPS().getFinished()) {
			std::cout << "START ENEMY " << std::endl;
			enemy.setInScene(true);
			enemy.Start();
		}
	}
}

