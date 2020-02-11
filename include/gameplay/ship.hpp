#ifndef __SHIP_H__
#define __SHIP_H__

#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <gameplay\enemy.hpp>

class Ship {

private:
	enum class MovementShip {
		ShipForward = 0,
		ShipBackward = 1,
		ShipLeft = 2,
		ShipRight = 3,
	};

	GameObject _go;
	std::vector<Bullet>& _bullets;
	std::vector<Enemy>& _enemies;
	glm::vec3 posShip = glm::vec3(0, 1, 0);
	glm::vec3 rotShip = glm::vec3(0, 0, 0);
	float angleShip = 0.0f;
	bool _shoot = false;
	float _angleMAX = 30.0f;
	float _angleShipStep = 1.0f;
	float speedM = 1.0;
	float speedR = 10.0;

	void handleInput(float dt);

	void shipMovement(MovementShip direction, float dt);

	void shipShoot();

	void checkCollisionBullet();
	

public:

	
	Ship(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies);

	void Start();

	void Update(float dt);

	glm::vec3 getPosition() { return posShip;  }

	GameObject getGO() { return _go; }

	void shipDraw();

};

#endif