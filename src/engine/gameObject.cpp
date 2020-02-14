#include "engine/gameObject.hpp"
#include <glm\ext\matrix_transform.hpp>


GameObject::GameObject(SceneGraph& sG, Node& node) : _sceneGraph(sG) {
	_position = glm::vec3(1.00f);
	//Translate(_position);
	_size = glm::vec3(1.00f);
	_rotation = glm::vec3(0.00f);
	_radians = 0.0f;
	_scale = glm::vec3(1.00f);
	//std::cout << idAsset << " " << material.getName() << " " << type << std::endl;
	_transform.Translate(_position);
	_transform.Rotate(_radians, _rotation);
	_transform.Scale(_scale);

	_idNode = sG.addNewNode(node);
	//std::cout << "idNode " << _idNode << std::endl;
}

void GameObject::Init() {
	//go = glm::mat4(1.0f);
	_transform.Init();
}

void GameObject::Translate(glm::vec3 position) {

	_transform.Translate(position);

	_position = position;
	//go = glm::translate(go, position);
}

void GameObject::Rotate(float angle, glm::vec3 rotation) {

	_transform.Rotate(angle, rotation);

	_rotation = rotation;
	//go = glm::rotate(go, glm::radians(angle), rotation);
}

void GameObject::Scale(glm::vec3 scale) {

	_transform.Scale(scale);

	_scale = scale;
	//go = glm::scale(go, scale);
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
	// Collision y-axis?
	bool collisionZ = (_position.z + _size.z + offset.z >= go.Position().z &&
		go.Position().z + go.getSize().z + offset.z >= _position.z) ||
		(_position.z - _size.z - offset.z <= go.Position().z &&
			go.Position().z - go.getSize().z - offset.z<= _position.z);
	// Collision only if on both axes


	//std::cout << collisionX << " " << collisionZ << std::endl;
	//std::cout << collisionX << " " << _go.Transform().getPosition().x << " " << go.Transform().getPosition().x << std::endl;

	return collisionX && collisionZ;
}

glm::vec2 GameObject::camBounds() {
	float top = (_sceneGraph.getCamera().getPosition().y - _position.y) * tan((_sceneGraph.getCamera().getFOV() * 0.5f) * 3.14f / 180.0f);
	float right = top * static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight();
	return glm::vec2(right,top);
}

bool GameObject::in_frustum(glm::vec3 offset) {
	//std::cout << _position.x <<","<<_position.y<<","<< _position.z << std::endl;
	
	//Pclip is the model-view-projection matrix to the object
	glm::vec4 Pclip =  _sceneGraph.getCamera().getProj() * _sceneGraph.getCamera().getViewMatrix() * glm::vec4(_position+offset, 1.0f);
	//std::cout << "infrustum " << Pclip.x<< " " << Pclip.y << " " <<Pclip.z << " " << Pclip.w << std::endl;
	return abs(Pclip.x) < Pclip.w &&
		abs(Pclip.y) < Pclip.w &&
		0 < Pclip.z &&
		Pclip.z < Pclip.w;
}

void GameObject::readyToDraw() {
	//_node.setDirtyFlag(true);
	//_node.setTrans(go);
	//std::cout << "SEND " << _idNode << std::endl;
	_sceneGraph.nodeReady(_idNode, _transform);
}

