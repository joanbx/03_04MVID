#include "..\..\include\gameplay\enemyManager.hpp"

EnemyManager::EnemyManager(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, Ship& player) : _enemies(enemies), _bullets(bullets), _player(player)
{

}

void EnemyManager::Start()
{

}

void EnemyManager::Update(float dt)
{
	//std::cout << _enemies.size() << std::endl;
	for (auto& enemy : _enemies) {
		enemy.Update(dt, _player.getPosition());
		//if (enemy.getDestroy() == true) {
		//	enemy.getPS().Update(dt, 2, enemy.getGO(), glm::vec3(0.0f, 0.0f, 0.0f));
		//}
	}
	for (auto& bullet : _bullets) {
		bullet.Update(dt);
	}
}

void EnemyManager::UpdatePS(float dt)
{
	for (auto& enemy : _enemies) {
		if (enemy.getDestroy() == true) {
			enemy.getPS().Update(dt, 3, enemy.getGO(), glm::vec3(0.0f, 0.0f, -0.5f));
		}
	}
}

