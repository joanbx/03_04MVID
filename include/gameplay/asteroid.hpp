#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include <engine\gameObject.hpp>
#include <GLFW\glfw3.h>

class Asteroid {


public:


	Asteroid(SceneGraph& sg, Node& node);

	void Start();

	void Update(float dt);

	void asteroidDraw();

	void setUse(bool use);



	bool getUsed() { return _used; }
	
	
	glm::vec3 getDirection() { return _direction; }
	GameObject& getGO() { return _go; }

private:

	GameObject _go;

	float _speedTranslation = 0.25f;
	float _speedRotation = 10.0f;
	glm::vec3 _modPos;
	
	glm::vec3 _posAsteroid = glm::vec3(0, 1, 0);
	glm::vec3 _rotAsteroid = glm::vec3(0, 0, 0);
	float _angleAsteroid = 0.0f;
	glm::vec3 _direction = glm::vec3(0, 0, 0);
	bool _used = false;
	bool _inFrustum = false;


};

#endif