#ifndef __BULLET_H__
#define __BULLET_H__

#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>

class Bullet {


public:
	
	enum class Bullettypes {
		Player,
		Enemy
	};

	Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes types);

	void Start();

	void Update(float dt);

	void bulletDraw();



	void setUse(bool use);

	void setPosition(glm::vec3& posBullet);

	
	GLboolean CheckCollisionXZ(GameObject go);
	

	bool getUsed() { return _used; }
	void setDirection(glm::vec3 direction);
	glm::vec3 getDirection() { return _direction; }
	GameObject& getGO() { return _go; }

private:

	GameObject _go;
	Bullet::Bullettypes& _type;
	float _speed = 0.0025f;
	glm::vec3 _posBullet = glm::vec3(0, 1, 0);
	glm::vec3 _rotBullet = glm::vec3(0, 0, 0);
	float _angleBullet = 0.0f;
	glm::vec3 _direction = glm::vec3(0,0,0);
	bool _used = false;
	bool _shoot = false;


	void pushDirection();
	void checkAutoDestroy();

};

#endif