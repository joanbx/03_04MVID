#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
#include <engine\sceneGraph.hpp>


const glm::vec3 K_positionGo = glm::vec3(0, 0, 0);

//Class GameObject: Link between node (scenegraph) and the gameplay objects
class GameObject {
private:
	glm::vec3 _position; //Position
	glm::vec3 _rotation; //Rotation
	float _radians; //angle
	glm::vec3 _scale; //scale
	glm::vec3 _size; //size
	SceneGraph& _sceneGraph; //SceneGraph
	int _idNode; //Node
	Transform _transform; //Transform
	

public:
	//Constructor
	GameObject(SceneGraph& sG, Node& node);
	~GameObject();
	GameObject(const GameObject&) = default;
	GameObject(GameObject&&) = default;
	GameObject& operator=(const GameObject&) = default;
	GameObject& operator=(GameObject&&) = default;

	//Init: Initialization
	void Init();
	//Translate: Translate object positions
	void Translate(glm::vec3 position);
	//Rotate: Rotate object with given angle and direction
	void Rotate(float radians, glm::vec3 direction);
	//Scale: Scale object
	void Scale(glm::vec3 scale);
	//setSize: Object Size
	void setSize(glm::vec3 size = glm::vec3(0.5f,0.5f,0.5f));
	//readyToDraw: Draw object -> Scenegraph
	void readyToDraw();

	//Position: return position
	glm::vec3 Position() { return _position; }
	//Rotation: return rotation
	glm::vec3 Rotation() { return _rotation; }
	//Radians: return angle
	float Radians() { return _radians; }
	//tScale: return scale
	glm::vec3 tScale() { return _scale; }
	//getSize: return size
	glm::vec3 getSize() { return _size; }
	//in_frustum: check if the object is in the frustrum of the camera
	bool in_frustum(glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
	//camBounds: Returns the bounds of the camera
	glm::vec2 camBounds();
	//CheckCollisionXZ: Check if there is a collision in 2D between this object and another one within an offset
	bool CheckCollisionXZ(GameObject go, glm::vec3 offset = glm::vec3(0.0f));
	//getProj: Return projection of the camera
	glm::mat4 getProj() { return _sceneGraph.getCamera().getProj(); }
	//getView: return view of the camera
	glm::mat4 getView() { return _sceneGraph.getCamera().getViewMatrix(); }
};

#endif