#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\node.hpp>
#include <engine\assets.hpp>
#include <engine\material.hpp>
#include <engine\shader.hpp>
#include <engine\window.hpp>
#include <engine\camera.hpp>

class SceneGraph {

private:

	struct nodes
	{
		int nodeID;
		Node& node;
		nodes(int id, Node& nd) : nodeID(id), node(nd) {}

	};
	int nodesCount = 0;
	std::vector<nodes> _nodes;
	
	Assets& _assets;
	Camera& _camera;
	//glm::mat4 _view;
	//glm::mat4 _proj;
	///Camera _camera;

public:
	SceneGraph(Assets& assets, Camera& camera);
	int addNewNode(Node& node);
	//void setViewProj(glm::mat4& view, glm::mat4& proj, Camera& camera);
	void updateNodes();
	void nodeReady(int id, Transform t);
	Node getNode(int node) { return _nodes[node].node; }

	Camera& getCamera() { return _camera; }
};

#endif