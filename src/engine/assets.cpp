#include <engine\assets.hpp>


AssetsGeometry::AssetsGeometry(const Geometry& geometry, const Texture& albedo, const Texture& specular, const Texture& normal) : _geometry(geometry), _albedo(albedo), _specular(specular), _normal(normal) {

}

//AssetsGeometry::AssetsGeometry(AssetsGeometry::typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral) {
//	if (type == AssetsGeometry::typeGeometry::isCube) {
//		const Cube _geometry(size);
//		//_geometry = geometry_;
//	}
//	if (type == AssetsGeometry::typeGeometry::isQuad) {
//		const Quad _geometry(size);
//	}
//	if (type == AssetsGeometry::typeGeometry::isSphere) {
//		const Sphere _geometry(size, 50, 50);
//		//_geometry = geometry_;
//	}
//
//	_textures.push_back(albedo);
//	_textures.push_back(difuse);
//	_textures.push_back(nomral);
//}
//
//AssetsGeometry::AssetsGeometry(AssetsGeometry::typeGeometry type, float size) {
//	if (type == AssetsGeometry::typeGeometry::isCube) {
//		const Cube _geometry(size);
//	}
//	if (type == AssetsGeometry::typeGeometry::isQuad) {
//		const Quad _geometry(size);
//	}
//	if (type == AssetsGeometry::typeGeometry::isSphere) {
//		const Sphere _geometry(size, 50, 50);
//	}
//
//}


//Assets::Assets(const Model& m) : _model(m){
//}

int Assets::addNewModel(const Model& m) {
	//_models.insert(std::map< std::string, Model >::value_type(name, Model(path)));
	//_models.insert(std::make_pair(name, Model(path)));
	int id = countAssets;
	//models_.push_back(m);
	_models.push_back(models(id, m));
	countAssets++;
	return id;
}

//void Assets::addNewTexture(char* path, std::string name) {
//	//_textures.insert(std::make_pair(name, Texture(path, Texture::Format::RGB)));
//	//_textures.insert(std::map< std::string, Texture >::value_type(name, Texture(path, Texture::Format::RGB)));
//}


//void Assets::addNewShader(char* pathVertex, char* pathFragment, std::string name) {
//	//_shaders.insert(std::make_pair(name, Shader(pathVertex, pathFragment)));
//	//_shaders.insert(std::map< std::string, Shader >::value_type(name, Shader(pathVertex, pathFragment)));
//}

int Assets::addNewGeometry(const Geometry& geometry, const Texture& albedo, const Texture& difuse, const Texture& nomral)
{
	int id = countAssets;
	geometries g(geometries(id, AssetsGeometry(geometry, albedo, difuse, nomral)));
	_geometries.push_back(g);
	countAssets++;
	return id;
}
//int Assets::addNewGeometry(Geometry geometry, float size)
//{
//	int id = countAssets;
//	_geometries.push_back(geometries(id, AssetsGeometry(geometry)));
//	countAssets++;
//	return id;
//}

//void Render(const Shader& shader) {
//	for (uint32_t i = 0; i < _model.meshes_.size(); i++)
//		_model.meshes_[i].render(shader);
//}

