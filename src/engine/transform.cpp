#include "engine/transform.hpp"
#include <glm\ext\matrix_transform.hpp>

Transform::Transform()
{
	_transform = glm::mat4(1.0f);
}

void Transform::Translate(glm::vec3 position) {
	_position = position;
	_transform = glm::translate(_transform, position);
}

void Transform::Rotate(float angle, glm::vec3 rotation) {
	_rotation = rotation;
	_transform = glm::rotate(_transform, glm::radians(angle), rotation);
}

void Transform::Scale(glm::vec3 scale) {
	_scale = scale;
	_transform = glm::scale(_transform, scale);
}



