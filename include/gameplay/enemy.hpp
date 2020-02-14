#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <engine\particleSystem.hpp>

class Enemy {

public:


	enum class EnemyTpye {
		EnemyVersion01,
		EnemyVersion02
	};

	Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, ParticleSystem& ps, EnemyTpye enemyType);

	void Start();

	void Update(float dt, glm::vec3& playerPos);

	

	void setInScene(bool isInScene);
	void setDestroy(bool destroy);
	void setDirection(glm::vec3 direction);
	void setChangeDirection(bool stoped);
	void setFrequencyShoot(float freq);
	void setSpeed(float speed);
	void setSpeedBullet(float speed);
	bool getInScene() { return _inScene; }
	GameObject& getGO() { return _go; }
	bool getDestroy() { return _destroy; }
	bool getChangeDirection() { return _changeDirection; }
	float getFrequencyShoot() { return _frequencyShoot; }
	glm::vec3 getDirectio() { return modPos; }
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
	std::vector<Bullet>& _bullets;
	ParticleSystem _ps;
	glm::vec3 posEnemy = glm::vec3(0, 1, -5.0f);
	glm::vec3 modPos = glm::vec3(0,0,0);
	glm::vec3 _rotEnemy = glm::vec3(0, 1, 0);
	float _frequencyShoot;
	float _speedBullet;
	float _angleEnemy;
	bool _shoot = false;
	bool _inScene = false;
	bool _prevInScene = false;
	float _speed;
	bool _destroy = false;
	bool _changeDirection = false;
	bool _inFrustum = false;
	float  time_start;
	float  time_elapsed;
	EnemyTpye _enemyType;

	void doDirection(float dt);
	void shoot(glm::vec3& playerPos);



};

#endif