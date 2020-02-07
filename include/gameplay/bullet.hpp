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

	

	

	bool getUsed() { return _used; }

	GameObject& getGO() { return _go; }

private:

	GameObject _go;
	Bullet::Bullettypes& _type;
	float _speed = 0.025f;
	glm::vec3 _posBullet = glm::vec3(0, 1, 0);
	glm::vec3 _rotBullet = glm::vec3(0, 0, 0);
	float _angleBullet = 0.0f;
	bool _used = false;
	bool _shoot = false;


	void pushForward();
	void checkDestroy();

};

#endif