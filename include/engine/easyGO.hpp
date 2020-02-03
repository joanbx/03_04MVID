#ifndef __EASYGO_H__
#define __EASYGO_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\shader.hpp>
#include <engine\geometry\geometry.hpp>
#include <engine\model.hpp>
#include <engine\texture.hpp>
#include <engine\sceneGraph.hpp>

const glm::vec3 K_positionGo_ = glm::vec3(0, 0, 0);


class EasyGO {

private:

	glm::vec3 _position;
	glm::vec3 _rotation;
	float _radians;
	glm::vec3 _scale;
	glm::vec3 _size;


public:
	
	glm::mat4 go;
	EasyGO(glm::vec3& position);
	//GameObject(SceneGraph& sG, Node& node);

	void Init();
	void Translate(const glm::vec3& position);
	void Rotate(const float& radians, const glm::vec3& direction);
	void Scale(const glm::vec3& scale);
	void setSize(const glm::vec3 size);


	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal);
	void Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	void Draw(glm::mat4& trans, const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);

	glm::vec3 Position() { return _position; }
	glm::vec3 Rotation() { return _rotation; }
	float Radians() { return _radians; }
	glm::vec3 tScale() { return _scale; }
	glm::vec3 getSize() { return _size; }
};

#endif