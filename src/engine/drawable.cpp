#include "engine/drawable.hpp"
#include <glm\ext\matrix_transform.hpp>
#include <iostream>


Draw::Draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj, const Geometry& geometry, Material& material) {
	
	material._shader.set("model", transform);
	material._shader.set("view", view);
	material._shader.set("proj", proj);

	glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
	material._shader.set("normalMat", normalMat);

	geometry.render();

}

Draw::Draw(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& proj, const Model& model, Material& material) {

	material._shader.set("model", transform);
	material._shader.set("view", view);
	material._shader.set("proj", proj);

	glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
	material._shader.set("normalMat", normalMat);

	model.render(material._shader);

}


Draw::Draw(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
	//shader.use();
	shader.set("model", transform);
	shader.set("view", view);
	shader.set("proj", proj);

	t_albedo.use(shader, "material.diffuse", 0);
	t_specular.use(shader, "material.specular", 1);
	t_normal.use(shader, "material.normal", 2);
	glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
	shader.set("normalMat", normalMat);


	geometry.render();
}

Draw::Draw(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", transform);
	shader.set("view", view);
	shader.set("proj", proj);

	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}


	geometry.render();
}

Draw::Draw(const glm::mat4& transform, const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", transform);
	shader.set("view", view);
	shader.set("proj", proj);
	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}