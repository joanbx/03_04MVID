#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\shader.hpp>
#include <engine\geometry\geometry.hpp>
#include <engine\model.hpp>
#include <engine\texture.hpp>

const glm::vec3 K_positionGo = glm::vec3(0, 0, 0);


class GameObject {

private:
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	float _radians;
	glm::vec3 _scale;
	glm::vec3 _size;
	//std::vector<Texture> _textures;
	//const char* _albedoPath;
	//Texture::Format _albedoFormat;
	//Texture _albedo(_albedoPath, _albedoFormat);
	//Texture _specular(NULL,NULL);
	//const Texture _normal;

public:
	glm::mat4 go = glm::mat4(1.0f);
	
	GameObject(const glm::vec3& position = K_positionGo);


	void Translate(glm::vec3 position);
	void Rotate(float radians, glm::vec3 direction);
	void Scale(glm::vec3 scale);
	void setSize(glm::vec3 size);
	
	
	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal);
	void Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);

	glm::vec3 Position() { return _position; }
	glm::vec3 Rotation() { return _rotation; }
	float Radians() { return _radians; }
	glm::vec3 tScale() { return _scale; }
	glm::vec3 getSize() { return _size; }
};

#endif