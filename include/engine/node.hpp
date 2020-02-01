#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
#include <engine\Material.hpp>
#include <engine\Drawable.hpp>

class Node {

private:

	Transform _transform;
	bool _dirtyFlag = false;
	bool _isModel = false;
	Model& _model;
	Material _material;

public:

	Node(std::string path);

	void setModel(Model& model);
	Model& getModel();

	Material& Node::getMaterial();
	void Node::setMaterial(Material& material);

	void setDirtyFlag(bool dirtyFlag);

	void drawNode(const glm::mat4& view, const glm::mat4& proj, Geometry geometry);
	void drawNode(const glm::mat4& view, const glm::mat4& proj);

	Transform Transform() { return _transform; }
	bool dirtyFlag() { return _dirtyFlag; }
};

#endif