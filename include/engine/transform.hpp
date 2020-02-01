#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>


//const glm::vec3 K_position = glm::vec3(0, 0, 0);


class Transform {

private:

	glm::vec3 _position;
	glm::vec3 _rotation;
	float _radians;
	glm::vec3 _scale;
	

public:
	glm::mat4 transform = glm::mat4(1.0f);

	//Transform(const glm::vec3& position = K_position);


	void Translate(glm::vec3 position);
	void Rotate(float radians, glm::vec3 direction);
	void Scale(glm::vec3 scale);


	glm::vec3 getPosition() { return _position; }
	glm::vec3 getRotation() { return _rotation; }
	float getRadians() { return _radians; }
	glm::vec3 getScale() { return _scale; }

};

#endif