#include "..\..\include\gameplay\enemyManager.hpp"

EnemyManager::EnemyManager(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, Ship& player) : _enemies(enemies), _bullets(bullets), _player(player)
{

}

void EnemyManager::Start()
{

}

void EnemyManager::Update(float dt)
{
	for (auto& enemy : _enemies) {
		enemy.Update(dt, _player.getPosition());
	}
	for (auto& bullet : _bullets) {
		bullet.Update(dt);
	}
}
