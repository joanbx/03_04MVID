#include "engine/transform.hpp"
#include <glm\ext\matrix_transform.hpp>

Transform::Transform()
{
	_transform = glm::mat4(1.0f);
}

void Transform::Init()
{
	_position.clear();
	_rotation.clear();
	_radians.clear();
	_scale.clear();
}


void Transform::Translate(glm::vec3& position) {
	_position.push_back(position);
}

void Transform::Rotate(float angle, glm::vec3& rotation) {
	_rotation.push_back(rotation);
	_radians.push_back(angle);
}

void Transform::Scale(glm::vec3& scale) {
	_scale.push_back(scale);
}


