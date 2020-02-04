#include <engine\sceneGraph.hpp>

SceneGraph::SceneGraph(Assets& assets) : _assets(assets){ }

int SceneGraph::addNewNode(Node& node) {
	int idNode = nodesCount;
	_nodes.push_back(nodes(idNode, node));
	std::cout << _nodes.size() << std::endl;
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

void SceneGraph::updateNodes(glm::mat4& view, glm::mat4& proj, glm::vec3& cameraPos) {
	//Group by shaders

	std::vector<std::string> shadersToUse;
	bool toAdd = false;

	for (auto& node : _nodes) {

		if (node.node.dirtyFlag()) {
			//Update position
		}
		
			
		if (shadersToUse.size() == 0) { 
			toAdd = true; 
		}
		else {
			for (auto& shaderName : shadersToUse) {
				if (shaderName != node.node.getMaterial().getName()) {
					toAdd = true;
					break;
				}
			}
		}
				
		if(toAdd) {
			shadersToUse.push_back(node.node.getMaterial().getName());
		}

	}
	
	//Draw by agrupations
	for (auto& s : shadersToUse) {
		bool initialized = false;
		//REMEMBER LIGHT
		for (auto& node : _nodes) {
			//std::cout << node.node.dirtyFlag() << std::endl;
			if (node.node.getMaterial().getName() == s && node.node.dirtyFlag()) {
				if (!initialized) {

					node.node.getMaterial()._shader.use();

					node.node.getMaterial().setMaterialLights();

					node.node.getMaterial()._shader.set("viewPos", cameraPos);


					initialized = true;
				}
				//draw
				//std::cout << "DRAW" << node.nodeID << std::endl;
				node.node.drawNode(view,proj,_assets);
				node.node.setDirtyFlag(false);
			}
			

		}

	}

}