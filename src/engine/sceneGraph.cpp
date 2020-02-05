#include <engine\sceneGraph.hpp>

SceneGraph::SceneGraph(Assets& assets) : _assets(assets){ }

int SceneGraph::addNewNode(Node& node) {
	int idNode = nodesCount;
	_nodes.push_back(nodes(idNode, node));
	//std::cout << _nodes.size() << std::endl;
	nodesCount++;
	return idNode;
}

//void SceneGraph::setViewProj(glm::mat4& view, glm::mat4& proj, Camera& camera) {
//	_view = view;
//	_proj = proj;
//	_camera = camera;
//}

void SceneGraph::nodeReady(int id, Transform t ) {
	
	//std::cout << _nodes.size() << std::endl;
	//std::cout << "ReadyNode " << _nodes[0].nodeID << std::endl;
	for (int i=0; i< _nodes.size(); ++i) {
		//std::cout << _nodes.size() << " " <<id << " " << _nodes[i].nodeID << std::endl;
		if (_nodes[i].nodeID == id) {
			//std::cout << "node found " << id<< std::endl;
			_nodes[i].node.setDirtyFlag(true);
			_nodes[i].node.setTrans(t);
			// _nodes[i].node._trans = t;
			
		}
	}
}

void SceneGraph::updateNodes(Camera& camera) {

	//First Pass

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
			//std::cout << node.node.dirtyFlag() << std::endl;
			if (node.node.dirtyFlag()) {
				if (!initializedDepth) {
					//std::cout << "FIRST PASS "  << std::endl;
					node.node.getMaterial()._shadow.FirstPass();
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
	glm::mat4 view = glm::lookAt(glm::vec3(0, 15.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1.0f)); //camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);
	
	for (auto& s : shadersToUse) {
		bool initialized = false;
		for (auto& node : _nodes) {
			//std::cout << node.node.dirtyFlag() << std::endl;
			if (node.node.dirtyFlag()) {
				if (!initialized) {
					//std::cout << "SECOND PASS " << std::endl;
					node.node.getMaterial()._shader.use();

					node.node.getMaterial().setMaterialLights();

					node.node.getMaterial()._shader.set("viewPos", camera.getPosition());
					node.node.getMaterial()._shader.set("view", view);
					node.node.getMaterial()._shader.set("proj", proj);
					node.node.getMaterial()._shadow.setDepthMap(node.node.getMaterial()._shader);

					initialized = true;
				}
				//draw
				//std::cout << "DRAW" << node.nodeID << std::endl;
				node.node.drawNode(_assets, false);
				node.node.setDirtyFlag(false);
			}

		}

	}

}