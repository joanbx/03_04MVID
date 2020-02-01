#include "engine/gameObject.hpp"
#include <glm\ext\matrix_transform.hpp>

GameObject::GameObject(glm::vec3 position)
{
	_transform.Translate(position);
	_size = glm::vec3(1.00f);
}


void GameObject::setSize(glm::vec3 size) {
	_size = size;
}

void GameObject::Visible(bool visible) {
	_isVisible = visible;

}

void GameObject::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
	//shader.use();
	shader.set("model", _transform.t);
	shader.set("view", view);
	shader.set("proj", proj);
	
	t_albedo.use(shader, "material.diffuse", 0);
	t_specular.use(shader, "material.specular", 1);
	t_normal.use(shader, "material.normal", 2);
	glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(go)));
	shader.set("normalMat", normalMat);
	

	geometry.render();
}

void GameObject::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", _transform.t);
	shader.set("view", view);
	shader.set("proj", proj);

	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(_transform.t)));
		shader.set("normalMat", normalMat);
	}


	geometry.render();
}

void GameObject::Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", _transform.t);
	shader.set("view", view);
	shader.set("proj", proj);
	
	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(_transform.t)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}

