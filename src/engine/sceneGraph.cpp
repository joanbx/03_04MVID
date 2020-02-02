#include <engine\sceneGraph.hpp>

SceneGraph::SceneGraph(Assets& assets) : _assets(assets){ }

int SceneGraph::addNewNode(Node& node) {
	int idNode = nodesCount;
	_nodes.push_back(nodes(idNode, node));
	nodesCount++;
	return idNode;
}

//void SceneGraph::setViewProj(glm::mat4& view, glm::mat4& proj, Camera& camera) {
//	_view = view;
//	_proj = proj;
//	_camera = camera;
//}

void SceneGraph::nodeReady(int id, glm::mat4 t) {
	for (int i=0; i< _nodes.size(); ++i) {
		if (_nodes[i].nodeID == id) {
			_nodes[i].node.setDirtyFlag(true);
			_nodes[i].node.setTrans(t);
		}
	}
}

void SceneGraph::updateNodes(glm::mat4& view, glm::mat4& proj, Camera& camera) {
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
				if (shaderName != node.node.getMaterial()._shader._name) {
					toAdd = true;
					break;
				}
			}
		}
				
		if(toAdd) {
			shadersToUse.push_back(node.node.getMaterial()._shader._name);
		}

	}
	
	//Draw by agrupations
	for (auto& s : shadersToUse) {
		bool initialized = false;
		//REMEMBER LIGHT
		for (auto& node : _nodes) {
			//std::cout << node.dirtyFlag() << std::endl;
			if (node.node.getMaterial()._shader._name == s && node.node.dirtyFlag()) {
				if (!initialized) {
					node.node.getMaterial()._shader.use();
					node.node.getMaterial()._shader.set("viewPos", camera.getPosition());
					node.node.getMaterial().setMaterialLights();
					initialized = true;
				}
				//draw
				node.node.drawNode(view,proj,_assets);
				node.node.setDirtyFlag(false);
			}


		}

	}

}