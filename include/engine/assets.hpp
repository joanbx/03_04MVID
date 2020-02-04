#ifndef __ASSETS_H__
#define __ASSETS_H__

#include <iostream>
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include <engine\model.hpp>
#include <engine\texture.hpp>
#include <engine\material.hpp>
#include <engine\geometry\geometry.hpp>
#include <engine\geometry\quad.hpp>
#include <engine\geometry\cube.hpp>
#include <engine\geometry\sphere.hpp>


class AssetsGeometry {

private:
	std::vector<Texture> _textures;
	const Texture& _albedo;
	const Texture& _specular;
	const Texture& _normal;
	const Geometry& _geometry;
	
public:
	
	
	
	enum typeGeometry {
		isQuad,
		isCube,
		isSphere
	};
	
	AssetsGeometry(const Geometry& geometry, const Texture& albedo, const Texture& specular, const Texture& normal);
	//AssetsGeometry(typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral);
	//AssetsGeometry(AssetsGeometry::typeGeometry type, float size);

	const Geometry& getGeometry() { return _geometry; }
	const std::vector<Texture> getTextures() { return _textures; }
	const Texture& getAlbedo() { return _albedo; }
	const Texture& getSpecular() { return _specular; }
	const Texture& getNormal() { return _normal; }
	//std::vector<Texture>& getTextures() { return _textures; }

};

class Assets {

	struct models
	{
		int assetID;
		const Model& model;
		models(int n, const Model& m) : assetID(n), model(m) {}

	};

	struct geometries
	{
		int assetID;
		const AssetsGeometry assetGeometry;
		geometries(int n, const AssetsGeometry g) : assetID(n), assetGeometry(g) {}

	};

private:
	

	std::vector<geometries> _geometries;
	std::vector<models> _models;

	//std::vector<Model> models_;

	int countAssets = 0;
	

public:
	
	//const Assets(const Model& m);
	int addNewModel(const Model& m);
	//void addNewTexture(char* path, std::string name);
	//void addNewShader(char* pathVertex, char* pathFragment,std::string name);
	int addNewGeometry(const Geometry& geometry, const Texture& albedo, const Texture& difuse, const Texture& nomral);
	//int addNewGeometry(AssetsGeometry::typeGeometry type, float size);

	//void Render(const Shader& shader);

	/*const Model& getModel_() {
		return models_[0];
	};*/

	//Shader getShader(std::string name) { return _shaders[name]; };
	const Model& getModel(int id) {
		for (auto& m : _models) {			
			if (m.assetID == id) {	
				return m.model;
			}	
		}
	};
	AssetsGeometry getAssetGeometry(int id) {
		for (auto& g : _geometries) {
			if (g.assetID == id)
				return g.assetGeometry;
		}
	}

};

#endif