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
	Geometry _geometry;
	
public:
	std::vector<Texture> _textures;
	enum typeGeometry {
		quad,
		cube,
		sphere
	};
	AssetsGeometry(typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral);
	AssetsGeometry(AssetsGeometry::typeGeometry type, float size);
	Geometry getGeometry() { return _geometry; }
	//std::vector<Texture>& getTextures() { return _textures; }

};

class Assets {

private:
	struct models
	{
		int assetID;
		const Model& model;
		models(int n, const Model& m) : assetID(n), model(m) {}

	};

	//std::map<std::string, Texture > _textures;
	//std::map<std::string, Shader > _shaders;
	//std::map <std::string, AssetsGeometry > _geometries;
	//std::vector<AssetsGeometry> _geometries;
	struct geometries
	{
		int assetID;
		const AssetsGeometry& assetGeometry;
		geometries(int n, const AssetsGeometry& g) : assetID(n), assetGeometry(g) {}

	};

	std::vector<geometries> _geometries;
	std::vector<models> _models;

public:
	

	void addNewModel(const Model& m, int id);
	//void addNewTexture(char* path, std::string name);
	//void addNewShader(char* pathVertex, char* pathFragment,std::string name);
	void addNewGeometry(AssetsGeometry::typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral, int id);
	void addNewGeometry(AssetsGeometry::typeGeometry type, float size, int id);

	//Shader getShader(std::string name) { return _shaders[name]; };
	const Model& getModel(int id) {
		for (auto& m : _models) {			
			if (m.assetID == id) {	
				return m.model;
			}	
		}
	};
	const AssetsGeometry& getAssetGeometry(int id) {
		for (auto& g : _geometries) {
			if (g.assetID == id)
				return g.assetGeometry;
		}
	}

};

#endif