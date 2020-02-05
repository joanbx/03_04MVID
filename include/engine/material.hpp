#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#include "engine/shader.hpp"
#include <engine\texture.hpp>
#include <engine\light.hpp>
#include <engine\shadow.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <string>

//const glm::vec3 K_ambient = glm::vec3(0.02f, 0.02f, 0.02f);
//const glm::vec3 K_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
//const glm::vec3 K_specular = glm::vec3(1.0f, 1.0f, 1.0f);
//const float K_shininess = 32;
//const std::string K_name = "standard";

class Material {
		
	private:
		//glm::vec3 _ambient;
		//glm::vec3 _diffuse;
		//glm::vec3 _specular;
		//float _shininess;
		////std::vector<Texture> _textures;
		//std::string _name;
		bool hasLightPorperties;
		
		DirLight _dirLight;
		std::vector<SpotLight> _spotLights;
		std::vector<PointLight> _pointLights;
		std::string _name;
		

	public:

		const Shader& _shader;
		Shadow& _shadow;

		Material(const Shader& shader, Shadow& shadow, DirLight dirLight, std::vector<SpotLight> spotLights, std::vector<PointLight> pointLights);
		//Material(const Shader& shader);
		Material() = delete;

		Material(const Material&) = default;
		Material& operator=(const Material&) = default;
		Material& operator=(Material&&) = default;

		//void setMaterial() ;
		void setMaterialTextures(const Texture& albedo, const Texture& specular, const Texture& normal) ;
		void setMaterialLights() ;

		std::string getName() {
			return _name;
		}

		DirLight getDirLight() { return _dirLight;  }
		std::vector<SpotLight> getSpotLights() { return _spotLights;  }
		std::vector<PointLight> getPointLights() { return _pointLights; }

		//Shader& getShader() { return _shader; }

		/*void setShader(Shader shader);
		void setAmbient(glm::vec3 ambient);
		void setDiffuse(glm::vec3 diffuse);
		void setSpecular(glm::vec3 specular);
		void setShininess(float shininess);
		void setName(std::string name);*/

		
		/*glm::vec3 getAmbient() { return _ambient; }
		glm::vec3 getDiffuse() { return _diffuse; }
		glm::vec3 getSpecular() { return _specular; }
		float getShininess() { return _shininess; }
		std::string getName() { return _name; }*/

};

#endif