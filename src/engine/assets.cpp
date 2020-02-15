#include <engine\assets.hpp>

//AssetsGeometry Constructor (const Geometry& geometry, const Texture& albedo, const Texture& specular, const Texture& normal)
AssetsGeometry::AssetsGeometry(const Geometry& geometry, const Texture& albedo, const Texture& specular, const Texture& normal) : _geometry(geometry), _albedo(albedo), _specular(specular), _normal(normal) {

}
//addNewModel: Add new model into the container and returns an id (integer)
int Assets::addNewModel(const Model& m) {
	//_models.insert(std::map< std::string, Model >::value_type(name, Model(path)));
	//_models.insert(std::make_pair(name, Model(path)));
	int id = countAssets;
	//models_.push_back(m);
	_models.push_back(models(id, m));
	countAssets++;
	return id;
}

//addNewGeometry:  Add new plain Geometry along its textures into the container and returns an id (integer)
int Assets::addNewGeometry(const Geometry& geometry, const Texture& albedo, const Texture& difuse, const Texture& nomral)
{
	int id = countAssets;
	geometries g(geometries(id, AssetsGeometry(geometry, albedo, difuse, nomral)));
	_geometries.push_back(g);
	countAssets++;
	return id;
}


