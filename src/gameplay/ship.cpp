#include <gameplay\ship.hpp>


Ship::Ship(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) : _go(sg, node), _bullets(bullets), _enemies(enemies){

}

void Ship::Start() {

}

void Ship::Update(float dt) {
	handleInput(dt);
	shipDraw();

	for (auto& b : _bullets) {
		b.Update(dt);
	}

	checkCollisionBullet();
}

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

	if (isMovingSide == false && angleShip!=0) {
		if (angleShip < 0.1f) {
			angleShip += _angleShipStep * (speedR *0.75) * dt;
			angleShip = std::min(angleShip, 0.0f);
		}
		else if (angleShip > 0.1f) {
			angleShip -= _angleShipStep * (speedR*0.75) * dt;
			angleShip = std::max(angleShip, 0.0f);
		}
		std::cout << angleShip << std::endl;
	}

	if (input->isKeyPressed(GLFW_KEY_SPACE) && _shoot == false) {
		_shoot = true;
		//posBullets.push_back(posShip);
		std::cout << "SHOOT" << std::endl;
		shipShoot();
		
	}
	else if (input->isKeyReleased(GLFW_KEY_SPACE) && _shoot == true) {
		_shoot = false;
	}
}

void Ship::shipShoot() {

	//Instance new bullet

	for (int i = 0; i < _bullets.size(); ++i) {
		if (_bullets[i].getUsed() == false) {
			//std::cout <<"BULLET No " << i << std::endl;
			_bullets[i].setUse(true);
			_bullets[i].setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
			_bullets[i].setPosition(posShip);
			break;
		}
	}

}

void Ship::shipMovement(MovementShip direction, float dt) {
		const float velocityM = speedM * dt;
		glm::vec3 front = glm::vec3(0, 0, -1);
		glm::vec3 right = glm::vec3(1, 0, 0);
		//Rotation
		const float velocityR = speedR * dt;

		switch (direction) {
			case MovementShip::ShipForward: 
				posShip += front * velocityM;
				break;
			case MovementShip::ShipBackward: 
				posShip -= front * velocityM; 
				break;
			case MovementShip::ShipLeft: 
				posShip -= right * velocityM; 
				rotShip = glm::vec3(0, 0, -1);
				angleShip += _angleShipStep * velocityR;
				if (angleShip > _angleMAX) angleShip = _angleMAX;
				break;
			case MovementShip::ShipRight: 
				posShip += right * velocityM; 
				rotShip = glm::vec3(0, 0, -1);
				angleShip -= _angleShipStep * velocityR;
				if (angleShip < -_angleMAX) angleShip = -_angleMAX;
				break;
			default:;
		}
}

void Ship::checkCollisionBullet() {
	for (auto& b : _bullets) {
		if (b.getUsed()) {
			for (auto& enemy : _enemies) {
				if (enemy.getInScene()) {
					if (b.CheckCollisionXZ(enemy.getGO()) == true) {
						b.setUse(false);
						enemy.setDestroy(true);
						enemy.setInScene(false);
					}
				}
			}
		}
	}

}

void Ship::shipDraw() {
	_go.Init();
	_go.Translate(posShip);
	//_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//_go.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	_go.Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (angleShip != 0.0f) _go.Rotate(angleShip, rotShip);
	//_go.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	_go.Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	_go.readyToDraw();
}