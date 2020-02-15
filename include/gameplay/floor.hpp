#ifndef __FLOOR_H__
#define __FLOOR_H__
#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>
//Class floor: Move quads with texture to give a movement effect in the scene
class Floor : public GameBehaviour {


public:


	Floor(std::vector<GameObject>& floors);
	~Floor();

	void Start() final;

	void Update(const float dt) final;



private:
	std::vector<GameObject>& _floors;
	float speed = 0.75f;
	float movement = 0;
	float _scale = 15.0f;
};

#endif