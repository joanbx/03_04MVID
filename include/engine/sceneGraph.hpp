#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\node.hpp>

class SceneGraph {

private:

	std::vector<Node> _nodes;

public:

	void addNewNode(Node node);

};

#endif