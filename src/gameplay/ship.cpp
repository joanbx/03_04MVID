#include <gameplay\ship.hpp>


Ship::Ship(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) : _go(sg, node), _bullets(bullets), _enemies(enemies){
	Start();
}

Ship::~Ship()
{
}

//Start ship 
void Ship::Start() {
	//Init size
	_go.setSize(glm::vec3(0.7f,1.0f,0.9f));
	//Init Variables
	_posShip = glm::vec3(0, 1, 0);
	_rotShip = glm::vec3(0, 0, 0);
	_angleShip = 0.0f;
	_shoot = false;
	_speedBullet = 3.5f;
	_angleMAX = 30.0f;
	_angleShipStep = 1.0f;
	_life = 3;
	_blinking = false;
	_stateBlink = true;
	_enemyKills = 0;
	_actualSeqBlink = 0;
	_speedM = 1.0;
	_speedR = 10.0;
	_extraBullet = false;
	//Init Bullets
	for (auto& bullet : _bullets) {
		bullet.Start();
	}
}
//Update ship
void Ship::Update(const float dt) {
	handleInput(dt);
	

	for (auto& b : _bullets) {
		b.Update(dt);
	}

	//Check if bullets collides with an enemy
	checkCollisionBullet();

	//Blinking mode: When blinking will be drawn every blinkingFreq interval during timeBlinking time
	if (_blinking) {
		float blinkingFreq = 0.5f;
		float timeBlinking = 2.0f;
		if ((int)(glfwGetTime() / blinkingFreq) % 2 == 1 && _stateBlink == false) {
			//std::cout << _actualSeqBlink << std::endl;
			_stateBlink = true;
			_actualSeqBlink++;
		}
		else if ((int)(glfwGetTime() / blinkingFreq) % 2 == 0 && _stateBlink) {
			_stateBlink = false;
		}

		if (_actualSeqBlink* blinkingFreq >= timeBlinking) {
			_actualSeqBlink = 0;
			_blinking = false;
			_stateBlink = true;
		}
	}
	//Draw when state blink is true (If it is not in blinking mode stateBlink will be always true)
	if(_stateBlink==true) {
		shipDraw();
	}
	
}
//Handles key input
void Ship::handleInput(float dt) {

	Input* input = Input::instance();
	bool isMovingSide = false;
	if (input->isKeyPressed(GLFW_KEY_W)) {
		shipMovement(MovementShip::ShipForward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		shipMovement(MovementShip::ShipBackward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		shipMovement(MovementShip::ShipLeft, dt);
		isMovingSide = true;
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		shipMovement(MovementShip::ShipRight, dt);
		isMovingSide = true;
	}

	//Not moving, then stabilize roll ship
	if (isMovingSide == false && _angleShip!=0) {
		if (_angleShip < 0.1f) {
			_angleShip += _angleShipStep * (_speedR *0.75) * dt;
			_angleShip = std::min(_angleShip, 0.0f);
		}
		else if (_angleShip > 0.1f) {
			_angleShip -= _angleShipStep * (_speedR*0.75) * dt;
			_angleShip = std::max(_angleShip, 0.0f);
		}
		//std::cout << angleShip << std::endl;
	}

	//If spacebar then shoot
	if (input->isKeyPressed(GLFW_KEY_SPACE) && _shoot == false) {
		_shoot = true;
		//posBullets.push_back(_posShip);
		std::cout << "SHOOT" << std::endl;
		shipShoot();
		
	}
	else if (input->isKeyReleased(GLFW_KEY_SPACE) && _shoot == true) {
		_shoot = false;
	}
}
//Action shoot
void Ship::shipShoot() {
	
	if (!_extraBullet) {
		//Instance new bullet (if there is an unused one)
		for (int i = 0; i < _bullets.size(); ++i) {
			if (_bullets[i].getUsed() == false) {
				//std::cout <<"BULLET No " << i << std::endl;
				_bullets[i].setSpeed(_speedBullet);
				_bullets[i].setUse(true);
				_bullets[i].setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
				_bullets[i].setPosition(_posShip);
				break;
			}
		}
	}
	else {
		uint32_t bulletsMax = 3;
		uint32_t bulletsUsed = 0;
		for (int i = 0; i < _bullets.size(); ++i) {
			if (_bullets[i].getUsed() == false) {
				//std::cout <<"BULLET No " << i << std::endl;
				_bullets[i].setUse(true);
				if(bulletsUsed == 0)
					_bullets[i].setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
				else if(bulletsUsed == 1)
					_bullets[i].setDirection(glm::vec3(0.5f, 0.0f, -1.0f));
				else if (bulletsUsed == 2)
					_bullets[i].setDirection(glm::vec3(-0.5f, 0.0f, -1.0f));
				_bullets[i].setPosition(_posShip);
				bulletsUsed++;
				if (bulletsUsed >= bulletsMax)
					break;
			}
		}

	}
	

}
//Controls movement of the ship
void Ship::shipMovement(MovementShip direction, float dt) {
	//Movement	
	const float velocityM = _speedM * dt;
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::vec3(1, 0, 0);
	//Rotation
	const float velocityR = _speedR * dt;

	//Control direction (movement + rotation-roll)
	switch (direction) {
		case MovementShip::ShipForward: 
			if (_go.in_frustum(glm::vec3(0.0, 0.0f, -_go.getSize().z))) _posShip += front * velocityM;
			break;
		case MovementShip::ShipBackward: 
			if (_go.in_frustum(glm::vec3(0.0, 0.0f, _go.getSize().z))) _posShip -= front * velocityM;
			break;
		case MovementShip::ShipLeft: 
			if(_go.in_frustum(glm::vec3(-_go.getSize().x,0.0f,0.0f))) _posShip -= right * velocityM;
			_rotShip = glm::vec3(0, 0, -1);
			_angleShip += _angleShipStep * velocityR;
			if (_angleShip > _angleMAX) _angleShip = _angleMAX;
			break;
		case MovementShip::ShipRight: 
			if (_go.in_frustum(glm::vec3(_go.getSize().x, 0.0f, 0.0f)))_posShip += right * velocityM;
			_rotShip = glm::vec3(0, 0, -1);
			_angleShip -= _angleShipStep * velocityR;
			if (_angleShip < -_angleMAX) _angleShip = -_angleMAX;
			break;
		default:;
	}
}
//Checks if there is a collision between a bullet and an enemy
void Ship::checkCollisionBullet() {
	for (auto& b : _bullets) {
		if (b.getUsed()) {
			//b.setAngle(-atan2(_posShip.z - b.getGO().Position().z, _posShip.x - b.getGO().Position().x) * 180 / 3.14159265359f );
			for (auto& enemy : _enemies) {
				if (enemy.getInScene()) {
					if (b.getGO().CheckCollisionXZ(enemy.getGO()) == true) {
						b.setUse(false);
						enemy.setDestroy(true);
						enemy.setInScene(false);
						_enemyKills++;
					}
				}
			}
		}
	}

}
void Ship::setPowerUp(bool power)
{
	_extraBullet = power;
}
//Destroy ship (or game over)
void Ship::setDestroy()
{	
	if (_blinking == false) {

		if (_life > 0) {
			_life--;
			_blinking = true;
			_posShip = glm::vec3(0,1,0);
		}
		else {
			std::cout << "DESTROY SHIP :(" << std::endl;
		}
	}

}
//Draw ship
void Ship::shipDraw() {
	_go.Init();
	_go.Translate(_posShip);
	_go.Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (_angleShip != 0.0f) _go.Rotate(_angleShip, _rotShip); //roll ship when moving sides
	_go.Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_go.readyToDraw();
}