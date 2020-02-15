#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#include "engine/shader.hpp"
#include <engine\texture.hpp>
#include <engine\light.hpp>
#include <engine\shadow.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <string>

//Class Material: Set Properties of the shader with a given shadow class, dirLght, spotlights and pointLights
//It is customized for our normal blinn phong shader with directional shadow and multiple lights
class Material {
		
	private:
		bool hasLightPorperties;
		
		DirLight _dirLight; //Dirlight
		std::vector<SpotLight> _spotLights; //Vector of spot lights
		std::vector<PointLight> _pointLights; //Vector of point lights
		Shadow& _shadow; //Shadow
		std::string _name; //Name
		

	public:

		const Shader& _shader; //Shader used for this material class
		
		//Constructor
		Material(const Shader& shader, Shadow& shadow, DirLight dirLight, std::vector<SpotLight> spotLights, std::vector<PointLight> pointLights);
		~Material();
		Material() = delete;

		Material(const Material&) = default;
		Material& operator=(const Material&) = default;
		Material& operator=(Material&&) = default;

		//setMaterialTextures: Set Textures of material
		void setMaterialTextures(const Texture& albedo, const Texture& specular, const Texture& normal) ;
		//setMaterialProperties: Set material properties according to the needed camera values into the shader
		void setMaterialProperties(glm::vec3& cameraPos, glm::mat4& view, glm::mat4& proj) ;

		//getName: return name
		std::string getName() {
			return _name;
		}
		//getDirLight: return dirLight
		DirLight getDirLight() { return _dirLight;  }
		//getSpotLights: return vectors of spot light
		std::vector<SpotLight> getSpotLights() { return _spotLights;  }
		//getPointLights: return vector of point light
		std::vector<PointLight> getPointLights() { return _pointLights; }
		//getShadow: return shadow
		Shadow& getShadow() { return _shadow; }
};

#endif