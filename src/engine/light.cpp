#include "engine/light.hpp"
#include "engine/shader.hpp"

const std::string prefixPoints = "pointLight[";
const std::string prefixSpots = "spotLight[";


DirLight::DirLight(const glm::vec3 direction,const glm::vec3 ambient,const glm::vec3 diffuse,const glm::vec3 specular)
	: _direction(direction), _ambient(ambient), _diffuse(diffuse), _specular(specular)
{ }

PointLight::PointLight(glm::vec3 position, const glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, glm::vec3 colorSphere)
	: _position(position), _ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic), _colorSphere(colorSphere)
{ }

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, const glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, glm::vec3 colorSphere)
		: _position(position), _direction(direction), _ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic), _cutOff(cutOff), _outerCutOff(outerCutOff), _colorSphere(colorSphere)
{ 
}

void DirLight::setShader(const Shader& s_phong) {
	s_phong.set("dirLight.direction", _direction);
	s_phong.set("dirLight.ambient", _ambient);
	s_phong.set("dirLight.diffuse", _diffuse);
	s_phong.set("dirLight.specular", _specular);
}

void PointLight::setShader(const Shader& s_phong, uint32_t i) {
	const std::string lightName = prefixPoints + std::to_string(i) + "].";
	s_phong.set((lightName + "position").c_str(), _position);
	s_phong.set((lightName + "ambient").c_str(), _ambient);
	s_phong.set((lightName + "diffuse").c_str(), _diffuse);
	s_phong.set((lightName + "specular").c_str(), _specular);
	s_phong.set((lightName + "constant").c_str(), _constant);
	s_phong.set((lightName + "linear").c_str(), _linear);
	s_phong.set((lightName + "quadratic").c_str(), _quadratic);
}

void SpotLight::setShader(const Shader& s_phong, uint32_t i) {
	const std::string lightName = prefixSpots + std::to_string(i) + "].";
	s_phong.set((lightName + "position").c_str(), _position);
	s_phong.set((lightName + "direction").c_str(), _direction);
	s_phong.set((lightName + "ambient").c_str(), _ambient);
	s_phong.set((lightName + "diffuse").c_str(), _diffuse);
	s_phong.set((lightName + "specular").c_str(), _specular);
	s_phong.set((lightName + "constant").c_str(), _constant);
	s_phong.set((lightName + "linear").c_str(), _linear);
	s_phong.set((lightName + "quadratic").c_str(), _quadratic);
	s_phong.set((lightName + "cutOff").c_str(), glm::cos(glm::radians(_cutOff)));
	s_phong.set((lightName + "outerCutOff").c_str(), glm::cos(glm::radians(_outerCutOff)));
}