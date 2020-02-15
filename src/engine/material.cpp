#include "engine/material.hpp"
#include <iostream>
#include <chrono>


Material::Material(const Shader& shader, Shadow& shadow, DirLight dirLight, std::vector<SpotLight> spotLights, std::vector<PointLight> pointLights) : _shader(shader), _shadow(shadow), _dirLight(dirLight), _pointLights(pointLights), _spotLights(spotLights) {
	hasLightPorperties = true;
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
		);

	_name = "mat" + std::to_string(ms.count()) + std::to_string(rand() % 1000); //TO BE IMPROVED
}

Material::~Material()
{
}


void Material::setMaterialTextures(const Texture& albedo, const Texture& specular, const Texture& normal)
{
	albedo.use(_shader, "material.diffuse", 0);
	specular.use(_shader, "material.specular", 1);
	normal.use(_shader, "material.normal", 2);
}


void Material::setMaterialProperties(glm::vec3& cameraPos, glm::mat4& view, glm::mat4& proj) {
	
	_shader.use();


	_shader.set("view", view);
	_shader.set("proj", proj);
	_shader.set("viewPos", cameraPos);

	if (hasLightPorperties) {

		_shader.set("material.shininess", 32);

		//DIRLIGHT
		_dirLight.setShader(_shader);
		
		/**/
		//SPOTLIGHTS
		for (int i = 0; i < _spotLights.size(); ++i) {
			_spotLights[i].setShader(_shader, i);
		}
		//POINTLIGHTS
		for (int i = 0; i < _pointLights.size(); ++i) {
			_pointLights[i].setShader(_shader, i);
		}
	}

	_shadow.setDepthMap(_shader);
	
}
