#include "engine/gameObject.hpp"
#include <glm\ext\matrix_transform.hpp>


GameObject::GameObject(SceneGraph& sG, Node& node) : _sceneGraph(sG) {
	_position = glm::vec3(1.00f);
	_size = glm::vec3(1.00f);
	_rotation = glm::vec3(0.00f);
	_radians = 0.0f;
	_scale = glm::vec3(1.00f);
	_transform.Translate(_position);
	_transform.Rotate(_radians, _rotation);
	_transform.Scale(_scale);

	_idNode = sG.addNewNode(node);
}

GameObject::~GameObject()
{
}

void GameObject::Init() {
	_transform.Init(); //Needed when we want to do a new draw
}

void GameObject::Translate(glm::vec3 position) {

	_transform.Translate(position);
	_position = position;
}

void GameObject::Rotate(float angle, glm::vec3 rotation) {

	_transform.Rotate(angle, rotation);
	_rotation = rotation;
}

void GameObject::Scale(glm::vec3 scale) {

	_transform.Scale(scale);
	_scale = scale;
}

void GameObject::setSize(glm::vec3 size) {
	_size = size;
}

bool GameObject::CheckCollisionXZ(GameObject go, glm::vec3 offset) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = (_position.x + _size.x + offset.x >= go.Position().x &&
		go.Position().x + go.getSize().x + offset.x >= _position.x) ||
		(_position.x - _size.x - offset.x <= go.Position().x &&
			go.Position().x - go.getSize().x - offset.x <= _position.x);
	// Collision z-axis?
	bool collisionZ = (_position.z + _size.z + offset.z >= go.Position().z &&
		go.Position().z + go.getSize().z + offset.z >= _position.z) ||
		(_position.z - _size.z - offset.z <= go.Position().z &&
			go.Position().z - go.getSize().z - offset.z<= _position.z);
	// Collision only if on both axes


	return collisionX && collisionZ;
}

glm::vec2 GameObject::camBounds() {
	float top = (_sceneGraph.getCamera().getPosition().y - _position.y) * tan((_sceneGraph.getCamera().getFOV() * 0.5f) * 3.14f / 180.0f);
	float right = top * static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight();
	return glm::vec2(right,top);
}

bool GameObject::in_frustum(glm::vec3 offset) {
	
	//Pclip is the model-view-projection matrix to the object
	glm::vec4 Pclip =  _sceneGraph.getCamera().getProj() * _sceneGraph.getCamera().getViewMatrix() * glm::vec4(_position+offset, 1.0f);
	return abs(Pclip.x) < Pclip.w &&
		abs(Pclip.y) < Pclip.w &&
		0 < Pclip.z &&
		Pclip.z < Pclip.w;
}

void GameObject::readyToDraw() {
	_sceneGraph.nodeReady(_idNode, _transform);
}

