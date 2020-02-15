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

//Class AssetGeometry: Contains textures and plain geometry
class AssetsGeometry {

private:
	std::vector<Texture> _textures;
	const Texture& _albedo;
	const Texture& _specular;
	const Texture& _normal;
	const Geometry& _geometry;
	
public:
	
	//Enum type geomtry: quad, cube, sphere
	enum typeGeometry {
		isQuad,
		isCube,
		isSphere
	};
	
	AssetsGeometry(const Geometry& geometry, const Texture& albedo, const Texture& specular, const Texture& normal);

	//getGeometry: return geometry
	const Geometry& getGeometry() { return _geometry; }
	//getTextures: return all textures
	const std::vector<Texture> getTextures() { return _textures; }
	//getAlbedo: return albedo texture
	const Texture& getAlbedo() { return _albedo; }
	//getSpecular: return specular texture
	const Texture& getSpecular() { return _specular; }
	//getNormal: return normal
	const Texture& getNormal() { return _normal; }

};
//Class Assets: Container of Models and Geometries
class Assets {
	//struct for models with ID
	struct models
	{
		int assetID;
		const Model& model;
		models(int n, const Model& m) : assetID(n), model(m) {}

	};
	//struct geometries with ID
	struct geometries
	{
		int assetID;
		const AssetsGeometry assetGeometry;
		geometries(int n, const AssetsGeometry g) : assetID(n), assetGeometry(g) {}

	};

private:
	

	std::vector<geometries> _geometries; //vector geometries
	std::vector<models> _models; //vector models

	int countAssets = 0; //Incremental ID
	

public:
	
	int addNewModel(const Model& m);

	int addNewGeometry(const Geometry& geometry, const Texture& albedo, const Texture& difuse, const Texture& nomral);

	//getModel: Get model with a given ID
	const Model& getModel(int id) {
		for (auto& m : _models) {			
			if (m.assetID == id) {	
				return m.model;
			}	
		}
	};
	//getAssetGeometry: Get AssetGeometry with a given id
	AssetsGeometry getAssetGeometry(int id) {
		for (auto& g : _geometries) {
			if (g.assetID == id)
				return g.assetGeometry;
		}
	}

};

#endif