#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <vector>


const glm::vec3 K_positionT = glm::vec3(0, 0, 0);

//Class transform: it contains container for position, rotation and scale
class Transform {

private:

	std::vector<glm::vec3> _position;
	std::vector<glm::vec3> _rotation;
	std::vector<float> _radians;
	std::vector<glm::vec3> _scale;

public:
	

	//Transform();
	
	//Initialize Transform
	void Init();
	//Translate: move position
	void Translate(glm::vec3& position);
	//Rotate: action rotation
	void Rotate(float radians, glm::vec3& direction);
	//Scale: Action scale
	void Scale(glm::vec3& scale);

	//getPosition
	std::vector<glm::vec3> getPosition() { return _position; }
	//getRotation
	std::vector<glm::vec3> getRotation() { return _rotation; }
	//getRadians
	std::vector<float> getRadians() { return _radians; }
	//getScale
	std::vector<glm::vec3> getScale() { return _scale; }

};

#endif