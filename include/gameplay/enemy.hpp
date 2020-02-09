#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <ctime>
#include <chrono>
#include <engine\particleSystem.hpp>

class Enemy {

public:

	enum class MovementType {
		simpleToLeft,
		simpleToRight
	};

	Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, ParticleSystem& ps);

	void Start();

	void Update(float dt, glm::vec3& playerPos);

	

	void setInScene(bool isInScene);
	void setDestroy(bool destroy);
	bool getInScene() { return _inScene; }
	GameObject& getGO() { return _go; }
	bool getDestroy() { return _destroy; }
	ParticleSystem& getPS() { return _ps; }
	void enemyDraw();

private:
	enum class MovementShip {
		ShipForward = 0,
		ShipBackward = 1,
		ShipLeft = 2,
		ShipRight = 3,
	};

	GameObject _go;
	MovementType _mt;
	std::vector<Bullet>& _bullets;
	ParticleSystem _ps;
	glm::vec3 posEnemy = glm::vec3(0, 3, -5.0f);
	glm::vec3 modPos = glm::vec3(0,0,0);
	glm::vec3 rotEnemy = glm::vec3(0, 0, 0);
	float angleEnemy = 0.0f;
	bool _shoot = false;
	bool _inScene = false;
	bool _prevInScene = false;
	float _speed = 0.1f;
	bool _destroy = false;
	bool _inFrustum = false;
	std::chrono::milliseconds  time_start;
	std::chrono::milliseconds  time_elapsed;
	std::uint32_t thresholdTimeShoot = 1000; //In ms

	void doDirection(float dt);
	void shoot(glm::vec3& playerPos);



};

#endif