#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include <engine\shader.hpp>
#include <engine\geometry\geometry.hpp>
#include <engine\model.hpp>
#include <engine\texture.hpp>
#include <engine\material.hpp>


class Draw {

private:

	//std::vector<Texture> _textures;
	//const Texture _normal;

public:

	Draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj, const Geometry& geometry, Material& material);
	Draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj, const Model& model, Material& material);
	Draw(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal);
	Draw(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal);
	Draw(const glm::mat4& transform, const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal);

};

class DrawModel {

private:

	//std::vector<Texture> _textures;
	//const Texture _normal;

public:

	DrawModel(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj, const Model& model, Material& material);


};

#endif