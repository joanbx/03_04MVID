#include <gameplay\enemy.hpp>


Enemy::Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets) : _go(sg, node), _bullets(bullets) {

}

void Enemy::Start() {

}

void Enemy::Update() {
	enemyDraw();
}

void Enemy::enemyDraw() {
	_go.Init();
	_go.Translate(posEnemy);
	_go.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	_go.setSize(glm::vec3(0.34f));
	_go.readyToDraw();
}