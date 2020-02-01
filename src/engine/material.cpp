#include "engine/material.hpp"

//Material::Material(const glm::vec3 ambient,const glm::vec3 diffuse,const glm::vec3 specular,const float shininess, const std::string name)
//	: _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess), _name(name)
//{
//
//}

void Material::setMaterial(Shader& shader, Texture& albedo, Texture& specular, Texture& normal)
{
	_shader = shader;
	albedo.use(shader, "material.diffuse", 0);
	specular.use(shader, "material.specular", 1);
	normal.use(shader, "material.normal", 2);
}

void Material::setShader(Shader shader) {
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
}