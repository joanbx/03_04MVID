#ifndef __SHIP_H__
#define __SHIP_H__

#include <engine\gameObject.hpp>
#include <engine/input.hpp>
#include <GLFW\glfw3.h>
#include <gameplay\bullet.hpp>
#include <gameplay\enemy.hpp>

//Player class (controls the player ship)
class Ship {

private:
	//Movement of the ship combinations
	enum class MovementShip {
		ShipForward = 0,
		ShipBackward = 1,
		ShipLeft = 2,
		ShipRight = 3,
	};

	GameObject _go; //GameObject
	std::vector<Bullet>& _bullets; //Bullets container
	std::vector<Enemy>& _enemies; //Enemies container
	glm::vec3 _posShip; //Position
	glm::vec3 _rotShip; //Rotation
	float _angleShip; //Angle
	bool _shoot; //is shooting
	float _angleMAX; // Max angle roll
	float _angleShipStep; // Step roll
	float _speedM; //Speed movement
	float _speedR; //Speed Rotation
	float _life; //actual life points
	bool _blinking; //Blinking? After get shot
	bool _stateBlink; //State blinking (true = draw)
	uint32_t _actualSeqBlink; //Blinking sequence
	uint32_t _enemyKills; //Number of enemies killed
	
	//Handles key input
	void handleInput(float dt);
	//Controls movement of the ship
	void shipMovement(MovementShip direction, float dt);
	//Action shoot
	void shipShoot();
	//Checks if there is a collision between a bullet and an enemy
	void checkCollisionBullet();

	
	

public:

	Ship(SceneGraph& sg, Node& node, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies);
	//Start ship 
	void Start();
	//Update ship
	void Update(float dt);

	//Get GameObject
	GameObject getGO() { return _go; }
	//Get position of the ship
	glm::vec3 getPosition() { return _posShip;  }
	//Get life ship
	uint32_t getLife() { return _life; }
	//Get number of kills
	uint32_t getKills() { return _enemyKills; }
	
	//Destroy ship (or game over)
	void setDestroy();

	//Draw ship
	void shipDraw();

};

#endif