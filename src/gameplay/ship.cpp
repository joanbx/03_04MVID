#include <gameplay\ship.hpp>



Ship::Ship(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies) : _go(sg, node), _bullets(bullets), _enemies(enemies){

}

void Ship::Start() {

}

void Ship::Update(float dt) {
	handleInput(dt);
	shipDraw();
}

void Ship::handleInput(float dt) {

	Input* input = Input::instance();

	if (input->isKeyPressed(GLFW_KEY_W)) {
		shipMovement(MovementShip::ShipForward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		shipMovement(MovementShip::ShipBackward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		shipMovement(MovementShip::ShipLeft, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		shipMovement(MovementShip::ShipRight, dt);
	}

	if (input->isKeyPressed(GLFW_KEY_SPACE) && _shoot == false) {
		_shoot = true;
		//posBullets.push_back(posShip);
		std::cout << "SHOOT" << std::endl;
	}
	else if (input->isKeyReleased(GLFW_KEY_SPACE) && _shoot == true) {
		_shoot = false;
	}
}

void Ship::shipShoot() {

	//Instance new bullet


}

void Ship::shipMovement(MovementShip direction, float dt) {
		const float velocityM = 1 * dt;
		glm::vec3 front = glm::vec3(0, 0, -1);
		glm::vec3 right = glm::vec3(1, 0, 0);
		//Rotation
		const float velocityR = 10 * dt;

		float angleMAX = 30.0f;
		float _angleShip = 1.0f;
	

		switch (direction) {
			case MovementShip::ShipForward: 
				posShip += front * velocityM;
				break;
			case MovementShip::ShipBackward: 
				posShip -= front * velocityM; 
				break;
			case MovementShip::ShipLeft: 
				posShip -= right * velocityM; 
				rotShip = glm::vec3(0, 1, 0);
				angleShip += _angleShip * velocityR;
				if (angleShip > angleMAX) angleShip = angleMAX;
				break;
			case MovementShip::ShipRight: 
				posShip += right * velocityM; 
				rotShip = glm::vec3(0, 1, 0);
				angleShip -= _angleShip * velocityR;
				if (angleShip < -angleMAX) angleShip = -angleMAX;
				break;
			default:;
		}


}

void Ship::shipDraw() {
	_go.Init();
	_go.Translate(posShip);
	_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleShip != 0.0f) _go.Rotate(angleShip, rotShip);
	_go.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	_go.readyToDraw();
}