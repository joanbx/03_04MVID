#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#include "engine/shader.hpp"
#include <engine\texture.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <string>

const glm::vec3 K_ambient = glm::vec3(0.02f, 0.02f, 0.02f);
const glm::vec3 K_diffuse = glm::vec3(0.2f, 0.2f, 0.2f);
const glm::vec3 K_specular = glm::vec3(1.0f, 1.0f, 1.0f);
const float K_shininess = 32;
const std::string K_name = "standard";

class Material {
		
		Shader _shader;
		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;
		float _shininess;
		//std::vector<Texture> _textures;
		std::string _name;

	public:
		//Material(const Shader shader,const glm::vec3 ambient = K_ambient,const glm::vec3 diffuse = K_diffuse,const glm::vec3 specular = K_specular,const float shininess = K_shininess, const std::string name= K_name);
		//Material(const glm::vec3 ambient = K_ambient, const glm::vec3 diffuse = K_diffuse, const glm::vec3 specular = K_specular, const float shininess = K_shininess, const std::string name = K_name);

		void setMaterial(Shader& shader, Texture& albedo, Texture& specular, Texture& normal);

		//void setMaterial();

		void setShader(Shader shader);
		void setAmbient(glm::vec3 ambient);
		void setDiffuse(glm::vec3 diffuse);
		void setSpecular(glm::vec3 specular);
		void setShininess(float shininess);
		void setName(std::string name);

		Shader getShader() { return _shader; }
		glm::vec3 getAmbient() { return _ambient; }
		glm::vec3 getDiffuse() { return _diffuse; }
		glm::vec3 getSpecular() { return _specular; }
		float getShininess() { return _shininess; }
		std::string getName() { return _name; }

};

#endif