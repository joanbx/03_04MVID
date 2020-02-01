#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
#include <engine\shader.hpp>
#include <engine\geometry\geometry.hpp>
#include <engine\model.hpp>
#include <engine\texture.hpp>

const glm::vec3 K_positionGo = glm::vec3(0, 0, 0);


class GameObject {

private:
	
	Transform _transform;
	glm::vec3 _size;
	bool _isVisible = true;

	//std::vector<Texture> _textures;
	//const char* _albedoPath;
	//Texture::Format _albedoFormat;
	//Texture _albedo(_albedoPath, _albedoFormat);
	//Texture _specular(NULL,NULL);
	//const Texture _normal;

public:
	
	GameObject(glm::vec3 position = K_positionGo);

	void setSize(glm::vec3 size);
	void Visible(bool visible);

	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	void Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal);
	void Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);

	Transform Transform() { return _transform; }
	bool isVisible() { return _isVisible; }
};

#endif