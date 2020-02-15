#ifndef __POWERUP_H__
#define __POWERUP_H__
#include <engine\gamebehaviour.hpp>
#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>
//Class powerUp
class PowerUp : public GameBehaviour {


public:


	PowerUp(SceneGraph& sg, Node& node);
	~PowerUp();

	void Start() final;

	void Update(const float dt) final;

	void setUsed(bool used);

	void PowerUpDraw();

	GameObject& getGO() { return _go; }
	bool getUsed() { return _used; }

private:
	GameObject _go;
	glm::vec3 _pu_position;
	bool _used;
	float _pu_rotation;
	float _speed = 0.75f;
};

#endif