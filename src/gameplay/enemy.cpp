#include <gameplay\enemy.hpp>


Enemy::Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, ParticleSystem& ps, EnemyTpye enemyType) : _go(sg, node), _bullets(bullets), _ps(ps), _enemyType(enemyType)
 {
	
	//Start();
}


void Enemy::Start() {

	//Set size
	switch (_enemyType) {
	case EnemyTpye::EnemyVersion01:
		_go.setSize(glm::vec3(0.34f));;
		break;
	case EnemyTpye::EnemyVersion02:
		_go.setSize(glm::vec3(0.5f));
		break;
	default:;

	}

	//initial pos
	glm::vec2 cambounds = _go.camBounds();
	float x = -cambounds.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*cambounds.x)));
	if (x < 0) {
		modPos = glm::vec3(0.5,0,1);
	}
	else {
		modPos = glm::vec3(-0.5, 0, 1);
	}
	posEnemy = glm::vec3(x, _go.Position().y, -cambounds.y - _go.getSize().z);

	//Init Variables
	_inFrustum = false;
	_inScene = true;
	_destroy = false;

	//Set particle system
	_ps.setFinished(false);
	_ps.Start();


	_go.Translate(posEnemy);
	//std::cout << "RandomX " <<posEnemy.x << std::endl;
}

void Enemy::Update(float dt, glm::vec3& playerPos) {
	if (_inScene) {
		doDirection(dt);				
		//std::cout << posEnemy.x << "," << posEnemy.y << " " << posEnemy.z << " " << _go.in_frustum() <<std :: endl;
		if (_go.in_frustum() == false && _inFrustum) {
			//_inScene = false;
			Start();
		}
		else if (!_inFrustum && _go.in_frustum()==true) {
			_inFrustum = true;
		}
		_angleEnemy = -atan2(playerPos.z - posEnemy.z, playerPos.x - posEnemy.x) * 180 / 3.14159265359f + 90.0f;
		shoot(playerPos);
		enemyDraw();

	}
	else if (_destroy) {
		//Start();
	}
	_prevInScene = _inScene;
		
}

void Enemy::setInScene(bool isInScene) {
	_inScene = isInScene;
}

void Enemy::setDestroy(bool destroy) {
	_destroy = destroy;
}

void Enemy::setDirection(glm::vec3 direction)
{
	modPos = direction;
}

void Enemy::setChangeDirection(bool stoped)
{
	_changeDirection = stoped;
}

void Enemy::setFrequencyShoot(float freq)
{
	_frequencyShoot = freq;
}

void Enemy::setSpeed(float speed)
{
	_speed = speed;
}

void Enemy::setSpeedBullet(float speed)
{
	_speedBullet = speed;
}

void Enemy::doDirection(float dt) {
	if(_changeDirection==false) posEnemy += modPos * _speed * dt;
	else posEnemy += -modPos * _speed * dt;
}

void Enemy::shoot(glm::vec3& playerPos) {
	if (_prevInScene == false) {
		//std::cout << _prevInScene << std::endl;
		time_start = glfwGetTime();
	}
	time_elapsed = glfwGetTime() - time_start;

	if (time_elapsed > _frequencyShoot) {
		for (int i = 0; i < _bullets.size(); ++i) {
			if (_bullets[i].getUsed() == false) {
				_bullets[i].setUse(true);
				_bullets[i].setSpeed(_speedBullet);
				_bullets[i].setDirection(glm::vec3(playerPos.x - posEnemy.x, posEnemy.y, playerPos.z - posEnemy.z));//(_go.Position()-glm::vec3(0,0,0));
				_bullets[i].setPosition(posEnemy);
				
				//std::cout << "shoot" << std::endl;
				time_start = glfwGetTime();
				break;
			}
		}
	}

	

}

void Enemy::enemyDraw() {
	_go.Init();
	_go.Translate(posEnemy);

	switch (_enemyType) {
		case EnemyTpye::EnemyVersion01:
			_go.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			_go.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
			break;
		case EnemyTpye::EnemyVersion02:
			_go.Scale(glm::vec3(0.002f, 0.002f, 0.002f));
			break;
		default:;

	}
	_go.Rotate(_angleEnemy, _rotEnemy);
	//_go.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//_go.Rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//_go.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	//_go.Scale(glm::vec3(0.01f, 0.01f, 0.01f));
	//_go.setSize(glm::vec3(0.34f));
	_go.readyToDraw();
}