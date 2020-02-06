#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>

class Enemy {

private:
	enum class MovementShip {
		ShipForward = 0,
		ShipBackward = 1,
		ShipLeft = 2,
		ShipRight = 3,
	};

	GameObject _go;
	std::vector<Bullet>& _bullets;
	glm::vec3 posEnemy = glm::vec3(0, 1, 0);
	glm::vec3 rotEnemy = glm::vec3(0, 0, 0);
	float angleEnemy = 0.0f;
	bool _shoot = false;

public:


	Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets);

	void Start();

	void Update(float dt);


	void enemyDraw();

};

#endif