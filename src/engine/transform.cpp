#include "engine/transform.hpp"
#include <glm\ext\matrix_transform.hpp>

//Transform::Transform(const glm::vec3& position)
//{
//	Translate(position);
//}

void Transform::Translate(glm::vec3 position) {
	_position = position;
	transform = glm::translate(transform, position);
}

void Transform::Rotate(float angle, glm::vec3 rotation) {
	_rotation = rotation;
	transform = glm::rotate(transform, glm::radians(angle), rotation);
}

void Transform::Scale(glm::vec3 scale) {
	_scale = scale;
	transform = glm::scale(transform, scale);
}



