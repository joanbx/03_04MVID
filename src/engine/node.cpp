#include <engine\node.hpp>


Node::Node(std::string path) {

	const Model object(path);
}

Material& Node::getMaterial() {
	return _material;
}

void Node::setMaterial(Material& material) {
	_material = material;
}

Model& Node::getModel() {
	return _model;
}

void Node::setModel(Model& model) {
	_model =  model;
}

Model& Node::getModel() {
	return _model;
}

void Node::setDirtyFlag(bool dirtyFlag) {
	_dirtyFlag = dirtyFlag;
}

void Node::drawNode(const glm::mat4& view, const glm::mat4& proj, Geometry geometry)
{
	Draw(_transform.transform, view, proj, geometry, _material);
}

void Node::drawNode(const glm::mat4& view, const glm::mat4& proj)
{
	Draw(_transform.transform, view, proj, _model, _material);
}