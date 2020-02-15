#ifndef __BULLET_H__
#define __BULLET_H__
#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>

//Class Bullet: Bullet (ammo) object
class Bullet : public GameBehaviour {


public:
	//Bullet types (player or enemy)
	enum class Bullettypes {
		isPlayer,
		isEnemy
	};
	//Constructor
	Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes types);
	~Bullet();
	
	void Start() final;

	void Update(const float dt) final;
	//bulletDraw: Ready to Draw bullet
	void bulletDraw();


	//setUse
	void setUse(bool use);
	//setStarted
	void setStarted(bool start);
	//setUpdated
	void setUpdated(bool updated);
	//setPosition
	void setPosition(glm::vec3& posBullet);
	//setAngle
	void setAngle(float angle);
	//setDirection
	void setDirection(glm::vec3 direction);
	//setSpeed
	void setSpeed(float speed);
	
	//getUsed
	bool getUsed() { return _used; }
	//getStarted
	bool getStarted() { return _started; }
	//getUpdated
	bool getUpdated() { return _updated; }
	//getDirection
	glm::vec3 getDirection() { return _direction; }
	//getGO: GameObject
	GameObject& getGO() { return _go; }

private:

	GameObject _go;
	Bullet::Bullettypes& _type;
	float _speed = 2.25f;
	glm::vec3 _posBullet;
	glm::vec3 _rotBullet = glm::vec3(0, 0, 0);
	float _angleBullet = 0.0f;
	glm::vec3 _direction = glm::vec3(0,0,0);
	bool _used = false;
	bool _shoot = false;
	bool _started = false;
	bool _updated = false;

	//pushDirection
	void pushDirection(float dt);
	//checkAutoDestroy
	void checkAutoDestroy();

};

#endif