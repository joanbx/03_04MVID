#include <engine\sceneGraph.hpp>

SceneGraph::SceneGraph(Assets& assets, Camera& camera) : _assets(assets), _camera(camera){ }

int SceneGraph::addNewNode(Node& node) {
	int idNode = nodesCount;
	_nodes.push_back(nodes(idNode, node));
	nodesCount++;
	return idNode;
}


void SceneGraph::nodeReady(int id, Transform t ) {
	
	for (int i=0; i< _nodes.size(); ++i) {
		if (_nodes[i].nodeID == id) {
			_nodes[i].node.setDirtyFlag(true);
			_nodes[i].node.setTrans(t);			
		}
	}
}

void SceneGraph::updateNodes() {

	

	//Group by shaders/Material
	std::vector<std::string> shadersToUse;
	bool toAdd = false;

	for (auto& node : _nodes) {	
		if (shadersToUse.size() == 0) { 
			toAdd = true; 
		}
		else {
			toAdd = false;
			for (auto& shaderName : shadersToUse) {
				if (shaderName != node.node.getMaterial().getName() && node.node.dirtyFlag() && toAdd == false) {
					toAdd = true;
				}
			}
		}
				
		if(toAdd) {
			shadersToUse.push_back(node.node.getMaterial().getName());
		}

	}
	
	//Draw by agrupations

	/**/
	//FIRST PASS - DEPTH
	for (auto& s : shadersToUse) {	
		bool initializedDepth = false;
		for (auto& node : _nodes) {
			if (node.node.dirtyFlag()) {
				if (!initializedDepth) {
					node.node.getMaterial().getShadow().FirstPass();
					initializedDepth = true;				
				}
				node.node.drawNode(_assets, true);
			}

		}
	}

	//SECOND PASS
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = _camera.getViewMatrix();
	glm::mat4 proj = _camera.getProj();
	
	for (auto& s : shadersToUse) {
		bool initialized = false;
		for (auto& node : _nodes) {
			if (node.node.dirtyFlag()) {
				if (!initialized) {
					node.node.getMaterial().setMaterialProperties(_camera.getPosition(), view, proj);
					initialized = true;
				}
				//draw node
				if (node.node.hasParent() == false) {
					node.node.drawNode(_assets, false);
					node.node.setDirtyFlag(false);
					//<-TODO: Following To be Tested
					for (auto& nodec : _nodes) {
						if (nodec.node.hasParent()) {
							if (nodec.node.getParent() == node.node.getId()) {
								nodec.node.drawNode(_assets, false);
								nodec.node.setDirtyFlag(false);
							}
						}

					}
				}
				
			}

		}

		//TEST DEPTH (Only for testing purposes)
		//_nodes[0].node.getMaterial()._shadow.setTestDepth();

	}

}