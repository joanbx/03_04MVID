#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
//#include <engine\shader.hpp>
//#include <engine\geometry\geometry.hpp>
//#include <engine\model.hpp>
#include <engine\transform.hpp>
//#include <engine\texture.hpp>
#include <engine\sceneGraph.hpp>


const glm::vec3 K_positionGo = glm::vec3(0, 0, 0);


class GameObject {
private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	float _radians;
	glm::vec3 _scale;
	glm::vec3 _size;
	SceneGraph& _sceneGraph;
	int _idNode;
	glm::mat4 go;
	Transform _transform;
	

public:
	//GameObject(const glm::vec3& position = K_positionGo);
	GameObject(SceneGraph& sG, Node& node);
	//GameObject(glm::vec3 poition);
	GameObject(const GameObject&) = default;
	GameObject(GameObject&&) = default;
	GameObject& operator=(const GameObject&) = default;
	GameObject& operator=(GameObject&&) = default;


	void Init();
	void Translate(glm::vec3& position);
	void Rotate(float radians, glm::vec3& direction);
	void Scale(glm::vec3& scale);
	void setSize(glm::vec3& size);
	void readyToDraw();

	//void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	//void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal);
	//void Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);

	glm::vec3 Position() { return _position; }
	glm::vec3 Rotation() { return _rotation; }
	float Radians() { return _radians; }
	glm::vec3 tScale() { return _scale; }
	glm::vec3 getSize() { return _size; }

	bool in_frustum(glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec2 camBounds();
	glm::mat4 getProj() { return _sceneGraph.getCamera().getProj(); }
	glm::mat4 getView() { return _sceneGraph.getCamera().getViewMatrix(); }
};

#endif