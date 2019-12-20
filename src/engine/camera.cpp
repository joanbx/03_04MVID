#include "engine/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
 : _position(position), _worldUp(up), _yaw(yaw), _pitch(pitch), _fov(k_FOV) {
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : _position(glm::vec3(posX, posY, posZ)), _worldUp(glm::vec3(upX, upY, upZ)), _yaw(yaw), _pitch(pitch), _fov(k_FOV) {
    updateCameraVectors();
}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::getViewMatrixCustom() const {
	//Integrar de forma manual la función lookAt
	//La función LookAt devuelve una mat4 tal que así:   
	// |Rx Ry Rz 0|		|1 0 0 -Px|
	// |Ux Uy Uz 0|	*	|0 1 0 -Py|
	// |Dx Dy Dz 0|		|0 0 1 -Pz|
	// |0  0  0  1|		|0 0 0  1 |
	//Rotación * Traslación (matriz)
	//R = vector derecha con el eje X
	//U = vector arriba con el eje Y
	//D = vector dirección con el eje Z
	//P = Posición de camera

	//Primero calculamos la traslación (posición camera) con la P
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -_position.x; // Columna 3, fila 0
	translation[3][1] = -_position.y; // Columna 3, fila 1
	translation[3][2] = -_position.z; // Columna 3, fila 2

	//Para la Rotación, necesitamos la R, la U y la D (descrito en el pdf de la asignatura..)
	glm::vec3 dir = _position + _front; //Dirección o Target

	glm::vec3 D = glm::normalize(_position - dir); //Eje Z
	glm::vec3 R = glm::normalize(glm::cross(glm::normalize(_up), D)); //Eje X
	glm::vec3 U = glm::cross(D, R); //Eje Y

	glm::mat4 rotation = glm::mat4(1.0f); 
	rotation[0][0] = R.x; // Columna 0, fila 0
	rotation[1][0] = R.y; // Columna 1, fila 0
	rotation[2][0] = R.z; // Columna 2, fila 0
	rotation[0][1] = U.x; // Columna 0, fila 1
	rotation[1][1] = U.y; // Columna 1, fila 1
	rotation[2][1] = U.z; // Columna 2, fila 1
	rotation[0][2] = D.x; // Columna 0, fila 2
	rotation[1][2] = D.y; // Columna 1, fila 2
	rotation[2][2] = D.z; // Columna 2, fila 2

	//Devolvemos la matriz tal y cómo se ha explicado al principio
	return rotation * translation;

}



float Camera::getFOV() const {
    return _fov;
}

glm::vec3 Camera::getPosition() const {
    return _position;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

void Camera::handleKeyboard(Movement direction, float dt) {
    const float velocity = k_Speed * dt;

    switch (direction) {
        case Movement::Forward: _position += _front * velocity; break;
        case Movement::Backward: _position -= _front * velocity; break;
        case Movement::Left: _position -= _right * velocity; break;
        case Movement::Right: _position += _right * velocity; break;
        default:;
    }
	if (_fps) _position.y = 0;
}

void Camera::setFPS(bool fps) {
	_fps = fps;
}

void Camera::handleMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    const float xoff = xoffset * k_Sensitivity;
    const float yoff = yoffset * k_Sensitivity;

    _yaw += xoff;
    _pitch += yoff;

    if (constrainPitch) {
        if (_pitch > 89.0f) _pitch = 89.0f;
        if (_pitch < -89.0f) _pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::handleMouseScroll(float yoffset) {
    if (_fov >= 1.0f && _fov <= 45.0f) _fov -= yoffset;
    if (_fov <= 1.0f) _fov = 1.0f;
    if (_fov >= 45.0f) _fov = 45.0f;
}

