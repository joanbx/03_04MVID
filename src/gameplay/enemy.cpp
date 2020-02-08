#include <gameplay\enemy.hpp>


Enemy::Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets) : _go(sg, node), _bullets(bullets) {
	
	Start();
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void Enemy::Start() {
	glm::vec3 size = glm::vec3(0.5f);

	_go.setSize(size);

	//initial pos
	glm::vec2 cambounds = _go.camBounds();
	float x = -cambounds.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*cambounds.x)));
	if (x < 0) {
		_mt = MovementType::simpleToRight;
		modPos = glm::vec3(0.5,0,1);
	}
	else {
		_mt = MovementType::simpleToLeft;
		modPos = glm::vec3(-0.5, 0, 1);
	}
	posEnemy = glm::vec3(x, _go.Position().y, -cambounds.y-size.y);
	std::cout << "RandomX " <<posEnemy.x << std::endl;
}

void Enemy::Update(float dt) {
	if (_inScene) {
		doDirection();				
		shoot();
		//std::cout << posEnemy.x << " " << posEnemy.y << " " << posEnemy.z << std::endl;
		
		enemyDraw();
	}

	_prevInScene = _inScene;
		
}

void Enemy::setInScene(bool isInScene) {
	_inScene = isInScene;

}

void Enemy::doDirection() {

	posEnemy += modPos * _speed;
}

void Enemy::shoot() {
	if (_prevInScene == false) {
		std::cout << _prevInScene << std::endl;
		time_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	}
	time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - time_start;
	std::cout << time_elapsed.count() << std::endl;

	if (time_elapsed.count() > thresholdTimeShoot) {	
		for (int i = 0; i < _bullets.size(); ++i) {
			if (_bullets[i].getUsed() == false) {
				_bullets[i].setUse(true);
				_bullets[i].setDirection(glm::vec3(0, 0, 1));//(_go.Position()-glm::vec3(0,0,0));
				_bullets[i].setPosition(posEnemy);
				std::cout << "shoot" << std::endl;
				time_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
				break;
			}
		}
	}

	

}

void Enemy::enemyDraw() {
	_go.Init();
	_go.Translate(posEnemy);
	_go.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	_go.setSize(glm::vec3(0.34f));
	_go.readyToDraw();
}