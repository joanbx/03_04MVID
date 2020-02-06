#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include "engine/shader.hpp"

const glm::vec3 K_position = glm::vec3(0, 0, 0);
const glm::vec3 K_direction = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 K_ambient = glm::vec3(0.02f, 0.02f, 0.02f);
const glm::vec3 K_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
const glm::vec3 K_specular = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 K_color = glm::vec3(1.0f, 1.0f, 1.0f);
const float K_constant = 1.0f;
const float K_linear = 0.1f;
const float K_quadratic = 0.064f;
const float K_cutOff = 30.0f;
const float K_outerCutOff = 40.0f;

class DirLight {
private:
	const glm::vec3& _direction;
	const glm::vec3& _ambient;
	const glm::vec3& _diffuse;
	const glm::vec3& _specular;
	

public:
	DirLight(const glm::vec3 direction = K_direction, const  glm::vec3 ambient = K_ambient, const  glm::vec3 diffuse = K_diffuse,const  glm::vec3 specular = K_specular);

	glm::vec3 getDirection() { return _direction; }
	glm::vec3 getAmbient() { return _ambient; }
	glm::vec3 getDiffuse() { return _diffuse; }
	glm::vec3 getSpecular() { return _specular; }

	void setShader(const Shader& s_phong);

};
class PointLight {
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _constant;
	float _linear;
	float _quadratic;
	glm::vec3 _colorSphere;

public:
	PointLight(glm::vec3 position = K_position, glm::vec3 ambient = K_ambient, glm::vec3 diffuse = K_diffuse, glm::vec3 specular = K_specular, float constant = K_constant, float linear = K_linear, float quadratic = K_quadratic, glm::vec3 colorSphere = K_color);

	glm::vec3 getPosition() { return _position; }
	glm::vec3 getAmbient() { return _ambient; }
	glm::vec3 getDiffuse() { return _diffuse; }
	glm::vec3 getSpecular() { return _specular; }
	float getConstant() { return _constant; }
	float getLinear() { return _linear; }
	float getQuadratic() { return _quadratic; }
	glm::vec3 getColorSphere() { return _colorSphere; }

	void setPosition(glm::vec3 position) { _position = position; }
	void setAmbient(glm::vec3 ambient) { _ambient = ambient; }
	void setDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
	void setSpecular(glm::vec3 specular) { _specular = specular; }
	void setLinear(float linear) { _linear = linear; }
	void setQuadratic(float quadratic) { _quadratic = quadratic; }

	void setShader(const Shader& s_phong, uint32_t i);

};
class SpotLight {
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _constant;
	float _linear;
	float _quadratic;
	float _cutOff;
	float _outerCutOff;
	glm::vec3 _colorSphere;
	glm::mat4 lightSpaceMatrix;

public:
	SpotLight(glm::vec3 position = K_position, glm::vec3 direction = K_direction, const glm::vec3 ambient = K_ambient, glm::vec3 diffuse = K_diffuse, glm::vec3 specular = K_specular, float constant = K_constant, float linear = K_linear, float quadratic = K_quadratic, float cutOff = K_cutOff, float outerCutOff = K_outerCutOff, glm::vec3 colorSphere = K_color);

	glm::vec3 getPosition() { return _position; }
	glm::vec3 getDirection() { return _direction; }
	glm::vec3 getAmbient() { return _ambient; }
	glm::vec3 getDiffuse() { return _diffuse; }
	glm::vec3 getSpecular() { return _specular; }
	float getConstant() { return _constant; }
	float getLinear() { return _linear; }
	float getQuadratic() { return _quadratic; }
	float getCutOff() { return _cutOff; }
	float getOuterCutOff() { return _outerCutOff; }
	glm::vec3 getColorSphere() { return _colorSphere; }

	void setPosition(glm::vec3 position) { _position = position; }
	void setDirection(glm::vec3 direction) { _direction = direction; }
	void setAmbient(glm::vec3 ambient) { _ambient = ambient; }
	void setDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
	void setSpecular(glm::vec3 specular) { _specular = specular; }
	void setLinear(float linear) { _linear = linear; }
	void setQuadratic(float quadratic) { _quadratic = quadratic; }
	void setCutOff(float cutOff) { _cutOff = cutOff; }
	void setOuterCutOff(float outerCutOff) { _outerCutOff = outerCutOff; }

	void setShader(const Shader& s_phong, uint32_t i);
};


#endif