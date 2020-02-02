#include <engine\node.hpp>
#include <iostream>


Node::Node(int idAsset,Material& material, Type type) : _idAsset(idAsset), _material(material), _type(type) {

}


void Node::setDirtyFlag(bool dirtyFlag) {
	_dirtyFlag = dirtyFlag;
}



void Node::drawNode(const glm::mat4& view, const glm::mat4& proj, Assets& asset)
{
	if (_type == Type::Model) {

			std::cout << "DRAW" << std::endl;
			Draw(_trans, _material._shader, asset.getModel(_idAsset), view, proj, true);
			//Draw(_transform.transform, view, proj, asset.getModel(_idAsset), _material);
		
		
	}
	else if(_type == Type::Geometry) { //Geometry
		//std::cout << _idAsset << std::endl;
		//if (asset.getAssetGeometry(_idAsset)._textures.size() > 0) {
			//std::cout << asset.getAssetGeometry(_idAsset)._textures.size() << std::endl;
		//}
		
		//if (asset.getAssetGeometry(_idAsset)._textures.size() == 3)
		//	_material.setMaterialTextures(asset.getAssetGeometry(_idAsset)._textures[0], asset.getAssetGeometry(_idAsset)._textures[1], asset.getAssetGeometry(_idAsset)._textures[2]);
		//Draw(_transform.transform, _material._shader, asset.getAssetGeometry(_idAsset).getGeometry(), view, proj, true);
	}
}

void Node::setTrans(glm::mat4 trans)
{
	_trans = trans;
}
