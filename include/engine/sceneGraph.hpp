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

//Class SceneGraph: Used to manage all the nodes present on the scene
class SceneGraph {

private:
	//Struct nodes
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

public:
	//Constructor
	SceneGraph(Assets& assets, Camera& camera);
	//addNewNode: Add new node and returns an ID integer
	int addNewNode(Node& node);
	//updateNodes: Update nodes in the game
	void updateNodes();
	//NodeReady
	void nodeReady(int id, Transform t);
	//getNode: Get node at a given integer id
	Node getNode(int node) { return _nodes[node].node; }
	//getCamera: return main camera
	Camera& getCamera() { return _camera; }
};

#endif