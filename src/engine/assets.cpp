#include <engine\assets.hpp>

AssetsGeometry::AssetsGeometry(AssetsGeometry::typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral) {
	if (type == AssetsGeometry::typeGeometry::cube) {
		Cube _geometry(size);
	}
	if (type == AssetsGeometry::typeGeometry::quad) {
		Quad _geometry(size);
	}
	if (type == AssetsGeometry::typeGeometry::sphere) {
		Sphere _geometry(size, 50, 50);
	}

	_textures.push_back(albedo);
	_textures.push_back(difuse);
	_textures.push_back(nomral);
}

AssetsGeometry::AssetsGeometry(AssetsGeometry::typeGeometry type, float size) {
	if (type == AssetsGeometry::typeGeometry::cube) {
		Cube _geometry(size);
	}
	if (type == AssetsGeometry::typeGeometry::quad) {
		Quad _geometry(size);
	}
	if (type == AssetsGeometry::typeGeometry::sphere) {
		Sphere _geometry(size, 50, 50);
	}

}

void Assets::addNewModel(const Model& m, int id) {
	//_models.insert(std::map< std::string, Model >::value_type(name, Model(path)));
	//_models.insert(std::make_pair(name, Model(path)));
	_models.push_back(models(id, m));
}

//void Assets::addNewTexture(char* path, std::string name) {
//	//_textures.insert(std::make_pair(name, Texture(path, Texture::Format::RGB)));
//	//_textures.insert(std::map< std::string, Texture >::value_type(name, Texture(path, Texture::Format::RGB)));
//}


//void Assets::addNewShader(char* pathVertex, char* pathFragment, std::string name) {
//	//_shaders.insert(std::make_pair(name, Shader(pathVertex, pathFragment)));
//	//_shaders.insert(std::map< std::string, Shader >::value_type(name, Shader(pathVertex, pathFragment)));
//}

void Assets::addNewGeometry(AssetsGeometry::typeGeometry type, float size, const Texture& albedo, const Texture& difuse, const Texture& nomral, int id)
{
	_geometries.push_back(geometries(id, AssetsGeometry(type, size, albedo, difuse, nomral)));
}
void Assets::addNewGeometry(AssetsGeometry::typeGeometry type, float size, int id)
{
	_geometries.push_back(geometries(id, AssetsGeometry(type, size)));
}

