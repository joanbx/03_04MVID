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
//Class DirLight: Creates a directional light with values: direction, ambient, diffuse and specular
class DirLight {
private:
	const glm::vec3& _direction;
	const glm::vec3& _ambient;
	const glm::vec3& _diffuse;
	const glm::vec3& _specular;
	

public:
	//Constructor DirLight
	DirLight(const glm::vec3 direction = K_direction, const  glm::vec3 ambient = K_ambient, const  glm::vec3 diffuse = K_diffuse,const  glm::vec3 specular = K_specular);
	//getDirection: return direction/position of the light
	glm::vec3 getDirection() { return _direction; }
	//getAmbient: return light ambient value
	glm::vec3 getAmbient() { return _ambient; }
	//getDiffuse: return light diffuse value
	glm::vec3 getDiffuse() { return _diffuse; }
	//getSpecular: return light specular value
	glm::vec3 getSpecular() { return _specular; }
	//setShader: Set values on a given shader
	void setShader(const Shader& s_phong);

};
//Class DirLight: Creates a Point light with values: position, ambient, diffuse, specular, constant, linear and quadratic
class PointLight {
	glm::vec3 _position;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _constant;
	float _linear;
	float _quadratic;
	glm::vec3 _colorSphere; //Only needed if we render it as a sphere for instance

public:
	//Constructir PointLight
	PointLight(glm::vec3 position = K_position, glm::vec3 ambient = K_ambient, glm::vec3 diffuse = K_diffuse, glm::vec3 specular = K_specular, float constant = K_constant, float linear = K_linear, float quadratic = K_quadratic, glm::vec3 colorSphere = K_color);
	//getPosition: return light position
	glm::vec3 getPosition() { return _position; }
	//getAmbient: return ambient light value
	glm::vec3 getAmbient() { return _ambient; }
	//getDiffuse: return diffuse light value
	glm::vec3 getDiffuse() { return _diffuse; }
	//getSpecular: return specular light value
	glm::vec3 getSpecular() { return _specular; }
	//getConstant: return constant light value
	float getConstant() { return _constant; }
	//getLinear: return linear light value
	float getLinear() { return _linear; }
	//getQuadratic: return quadratic light value
	float getQuadratic() { return _quadratic; }
	//getColorSphere: Color of the sphere
	glm::vec3 getColorSphere() { return _colorSphere; }

	//setPosition: set new point light position 
	void setPosition(glm::vec3 position) { _position = position; }
	//setAmbient: set new point light ambient value 
	void setAmbient(glm::vec3 ambient) { _ambient = ambient; }
	//setDiffuse: set new point light diffuse value 
	void setDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
	//setSpecular: set new point light specular value 
	void setSpecular(glm::vec3 specular) { _specular = specular; }
	//setLinear: set new point light linear value 
	void setLinear(float linear) { _linear = linear; }
	//setQuadratic: set new point light quadratic value 
	void setQuadratic(float quadratic) { _quadratic = quadratic; }
	//setShader: Set values on a given shader
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
	//Constructir SpotLight
	SpotLight(glm::vec3 position = K_position, glm::vec3 direction = K_direction, const glm::vec3 ambient = K_ambient, glm::vec3 diffuse = K_diffuse, glm::vec3 specular = K_specular, float constant = K_constant, float linear = K_linear, float quadratic = K_quadratic, float cutOff = K_cutOff, float outerCutOff = K_outerCutOff, glm::vec3 colorSphere = K_color);
	//getPosition: return light position
	glm::vec3 getPosition() { return _position; }
	//getDirection: return light direction
	glm::vec3 getDirection() { return _direction; }
	//getAmbient: return light ambient value
	glm::vec3 getAmbient() { return _ambient; }
	//getDiffuse: return light diffuse value
	glm::vec3 getDiffuse() { return _diffuse; }
	//getSpecular: return light specular value
	glm::vec3 getSpecular() { return _specular; }
	//getConstant: return light constant value
	float getConstant() { return _constant; }
	//getLinear: return light linear value
	float getLinear() { return _linear; }
	//getQuadratic: return light quadratic value
	float getQuadratic() { return _quadratic; }
	//getCutOff: return light cutoff value
	float getCutOff() { return _cutOff; }
	//getOuterCutOff: return light outer cut off value
	float getOuterCutOff() { return _outerCutOff; }
	//getColorSphere: return light color sphere
	glm::vec3 getColorSphere() { return _colorSphere; }

	//setPosition: set new spot light position 
	void setPosition(glm::vec3 position) { _position = position; }
	//setDirection: set new spot light direction 
	void setDirection(glm::vec3 direction) { _direction = direction; }
	//setAmbient: set new spot light ambient value 
	void setAmbient(glm::vec3 ambient) { _ambient = ambient; }
	//setDiffuse: set new spot light diffuse value 
	void setDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
	//setSpecular: set new spot light specular value 
	void setSpecular(glm::vec3 specular) { _specular = specular; }
	//setLinear: set new point light linear value 
	void setLinear(float linear) { _linear = linear; }
	//setQuadratic: set new spot light quadratic value 
	void setQuadratic(float quadratic) { _quadratic = quadratic; }
	//setCutOff: set new spot light cutoff value 
	void setCutOff(float cutOff) { _cutOff = cutOff; }
	//setOuterCutOff: set new spot light outer cutoff value 
	void setOuterCutOff(float outerCutOff) { _outerCutOff = outerCutOff; }
	//setShader: Set values on a given shader
	void setShader(const Shader& s_phong, uint32_t i);
};


#endif