#include <gameplay\enemy.hpp>


Enemy::Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets) : _go(sg, node), _bullets(bullets) {

}

void Enemy::Start() {

}

void Enemy::Update(float dt) {
	enemyDraw();
}

void Enemy::enemyDraw() {
	_go.Init();
	_go.Translate(posEnemy);
	_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleEnemy != 0.0f) _go.Rotate(angleEnemy, rotEnemy);
	_go.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	_go.readyToDraw();
}