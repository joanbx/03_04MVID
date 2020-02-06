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

private:


	GameObject _go;
	Bullet::Bullettypes& _type;
	glm::vec3 posBullet = glm::vec3(0, 1, 0);
	glm::vec3 rotBullet = glm::vec3(0, 0, 0);
	float angleBullet = 0.0f;
	bool _shoot = false;

};

#endif