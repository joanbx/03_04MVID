#include "engine/gameObject.hpp"
#include <glm\ext\matrix_transform.hpp>

//GameObject::GameObject(const glm::vec3& position)
//{
//	_size = glm::vec3(1.00f);
//	Translate(position);
//}

GameObject::GameObject(SceneGraph& sG, Node& node) : _sceneGraph(sG) {
	_position = glm::vec3(1.00f);
	//Translate(_position);
	_size = glm::vec3(1.00f);
	_rotation = glm::vec3(0.00f);
	_radians = 0.0f;
	_scale = glm::vec3(1.00f);
	//std::cout << idAsset << " " << material.getName() << " " << type << std::endl;
	_transform.Translate(_position);
	_transform.Rotate(_radians, _rotation);
	_transform.Scale(_scale);

	_idNode = sG.addNewNode(node);
	//std::cout << "idNode " << _idNode << std::endl;
}

void GameObject::Init() {
	go = glm::mat4(1.0f);
	_transform.Init();
}

void GameObject::Translate(glm::vec3& position) {

	_transform.Translate(position);

	_position = position;
	go = glm::translate(go, position);
}

void GameObject::Rotate(float angle, glm::vec3& rotation) {

	_transform.Rotate(angle, rotation);

	_rotation = rotation;
	go = glm::rotate(go, glm::radians(angle), rotation);
}

void GameObject::Scale(glm::vec3& scale) {

	_transform.Scale(scale);

	_scale = scale;
	go = glm::scale(go, scale);
}

void GameObject::setSize(glm::vec3& size) {
	_size = size;
}

void GameObject::readyToDraw() {
	//_node.setDirtyFlag(true);
	//_node.setTrans(go);
	//std::cout << "SEND " << _idNode << std::endl;
	_sceneGraph.nodeReady(_idNode, _transform);
}

void GameObject::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, const Texture& t_albedo, const Texture& t_specular, const Texture& t_normal) {
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

void GameObject::Draw(const Shader& shader, const Geometry& geometry, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
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

void GameObject::Draw(const Shader& shader, const Model& model, const glm::mat4& view, const glm::mat4& proj, bool isNormal) {
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