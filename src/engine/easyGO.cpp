#include "engine/easyGO.hpp"
#include <glm\ext\matrix_transform.hpp>

EasyGO::EasyGO(glm::vec3& position) : _position(position), go(glm::mat4(1.0f))
{
	_size = glm::vec3(1.00f);
	Translate(position);
}

void EasyGO::Init() {
	go = glm::mat4(1.0f);
}

void EasyGO::Translate(const glm::vec3& position) {
	_position = position;
	go = glm::translate(go, position);
}

void EasyGO::Rotate(const float& angle, const glm::vec3& rotation) {
	_rotation = rotation;
	go = glm::rotate(go, glm::radians(angle), rotation);
}

void EasyGO::Scale(const glm::vec3& scale) {
	_scale = scale;
	go = glm::scale(go, scale);
}

void EasyGO::setSize(glm::vec3 size) {
	_size = size;
}



void EasyGO::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
	//shader.use();
	shader.set("model", go);
	shader.set("view", view);
	shader.set("proj", proj);

	t_albedo.use(shader, "material.diffuse", 0);
	t_specular.use(shader, "material.specular", 1);
	t_normal.use(shader, "material.normal", 2);
	glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(go)));
	shader.set("normalMat", normalMat);


	geometry.render();
}

void EasyGO::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", go);
	shader.set("view", view);
	shader.set("proj", proj);

	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(go)));
		shader.set("normalMat", normalMat);
	}


	geometry.render();
}

void EasyGO::Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", go);
	shader.set("view", view);
	shader.set("proj", proj);

	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(go)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}

void EasyGO::Draw(glm::mat4& trans, const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
	//shader.use();
	shader.set("model", trans);
	shader.set("view", view);
	shader.set("proj", proj);

	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(trans)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}



