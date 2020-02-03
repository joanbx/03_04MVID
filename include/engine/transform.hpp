#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <vector>


const glm::vec3 K_positionT = glm::vec3(0, 0, 0);


class Transform {

private:

	std::vector<glm::vec3> _position;
	std::vector<glm::vec3> _rotation;
	std::vector<float> _radians;
	std::vector<glm::vec3> _scale;
	glm::mat4 _transform = glm::mat4(1.0f);

public:
	

	Transform();

	void Init();
	void Translate(glm::vec3& position);
	void Rotate(float radians, glm::vec3& direction);
	void Scale(glm::vec3& scale);

	glm::mat4 getTransform() { return _transform; }
	std::vector<glm::vec3> getPosition() { return _position; }
	std::vector<glm::vec3> getRotation() { return _rotation; }
	std::vector<float> getRadians() { return _radians; }
	std::vector<glm::vec3> getScale() { return _scale; }

};

#endif