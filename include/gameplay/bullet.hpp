#ifndef __BULLET_H__
#define __BULLET_H__
#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>

class Bullet : public GameBehaviour {


public:
	
	enum class Bullettypes {
		isPlayer,
		isEnemy
	};

	Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes types);
	~Bullet();

	void Start() final;

	void Update(const float dt) final;

	void bulletDraw();



	void setUse(bool use);
	void setStarted(bool start);
	void setUpdated(bool updated);
	void setPosition(glm::vec3& posBullet);
	void setAngle(float angle);
	void setDirection(glm::vec3 direction);
	void setSpeed(float speed);
	

	bool getUsed() { return _used; }
	bool getStarted() { return _started; }
	bool getUpdated() { return _updated; }
	glm::vec3 getDirection() { return _direction; }
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

	void pushDirection(float dt);
	void checkAutoDestroy();

};

#endif