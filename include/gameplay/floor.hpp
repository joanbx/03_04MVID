#ifndef __FLOOR_H__
#define __FLOOR_H__

#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>

class Floor {


public:


	Floor(std::vector<GameObject>& floors);

	void Start();

	void Update(float dt);



private:
	std::vector<GameObject>& _floors;
	float speed = 0.75f;
	float movement = 0;
	float _scale = 15.0f;
};

#endif