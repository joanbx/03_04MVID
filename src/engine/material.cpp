#include "engine/material.hpp"
#include <iostream>

//Material::Material(const glm::vec3 ambient,const glm::vec3 diffuse,const glm::vec3 specular,const float shininess, const std::string name)
//	: _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess), _name(name)
//{
//
//}

Material::Material(const Shader& shader, DirLight& dirLight, std::vector<SpotLight>& spotLights, std::vector<PointLight>& pointLights) : _shader(shader) , _dirLight(dirLight), _pointLights(pointLights) {
	hasLightPorperties = true;
}

Material::Material(const Shader& shader) : _shader(shader) {
}

void Material::setMaterialTextures(Texture& albedo, Texture& specular, Texture& normal)
{
	albedo.use(_shader, "material.diffuse", 0);
	specular.use(_shader, "material.specular", 1);
	normal.use(_shader, "material.normal", 2);
}

void Material::setMaterial() {

}

void Material::setMaterialLights() {
	//DIRLIGHT
	if (hasLightPorperties) {
		_shader.set("Light.direction", _dirLight.getDirection());
		_shader.set("Light.ambient", _dirLight.getAmbient());
		_shader.set("Light.diffuse", _dirLight.getDiffuse());
		_shader.set("Light.specular", _dirLight.getSpecular());
		_shader.set("material.shininess", 128);

		for (int i = 0; i < _spotLights.size(); ++i) {
			_spotLights[i].setShader(_shader, i);
		}
		for (int i = 0; i < _pointLights.size(); ++i) {
			_pointLights[i].setShader(_shader, i);
		}

	}
	
}

/*void Material::setShader(Shader shader) {
	_shader = shader;
}
void Material::setAmbient(glm::vec3 ambient) {
	_ambient = ambient;
}
void Material::setDiffuse(glm::vec3 diffuse) {
	_diffuse = diffuse;
}
void Material::setSpecular(glm::vec3 specular) {
	_specular = specular;
}
void Material::setShininess(float shininess) {
	_shininess = shininess;
}
void Material::setName(std::string name) {
	_name = name;
}*/