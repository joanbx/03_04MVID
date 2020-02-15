#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <engine\particleSystem.hpp>
//Class enemy: Enemy object
class Enemy : public GameBehaviour {

public:


	enum class EnemyTpye {
		EnemyVersion01,
		EnemyVersion02
	};

	Enemy(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, ParticleSystem& ps, EnemyTpye enemyType);
	~Enemy();

	void Start() final;

	void Update(const float dt) final;
	//enemyDraw: Ready to draw node
	void enemyDraw();

	//setInScene: visible
	void setInScene(bool isInScene);
	//setDestroy
	void setDestroy(bool destroy);
	//setDirection: vec3
	void setDirection(glm::vec3 direction);
	//setChangeDirection: bool
	void setChangeDirection(bool stoped);
	//setFrequencyShoot: float
	void setFrequencyShoot(float freq);
	//setSpeed: float
	void setSpeed(float speed);
	//setSpeedBullet: float
	void setSpeedBullet(float speed);
	//setPlayerPos: vec3
	void setPlayerPos(glm::vec3& position);
	
	//getBullets
	std::vector<Bullet>& getBullets() { return _bullets; }
	//getInScene: visible
	bool getInScene() { return _inScene; }
	//getGO: GameObject
	GameObject& getGO() { return _go; }
	//getDestroy: bool
	bool getDestroy() { return _destroy; }
	//getChangeDirection: bool
	bool getChangeDirection() { return _changeDirection; }
	//getFrequencyShoot
	float getFrequencyShoot() { return _frequencyShoot; }
	//getDirectio
	glm::vec3 getDirectio() { return modPos; }
	//getPS: Particle System
	ParticleSystem& getPS() { return _ps; }
	
	

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
	glm::vec3 _playerPos = glm::vec3(0, 1, 0);
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

	//doDirection
	void doDirection(float dt);
	//shoot
	void shoot();



};

#endif