#include <engine\node.hpp>
#include <iostream>
#include <glm\ext\matrix_transform.hpp>


Node::Node(int idAsset,Material& material, Type type) : _idAsset(idAsset), _material(material), _type(type)  {

}


void Node::setDirtyFlag(bool dirtyFlag) {
	_dirtyFlag = dirtyFlag;
}



void Node::drawNode(Assets& asset, bool isDepth)
{

	//Do Transform

	glm::mat4 m = glm::mat4(1.0);

	if (_transform.getPosition().size() > 0) {
		for (auto& _t : _transform.getPosition()) {
			m = glm::translate(m, _t);
		}
	}
	
	if (_transform.getRotation().size() > 0) {
		for (int i = 0; i < _transform.getRotation().size(); ++i) {
			m = glm::rotate(m, glm::radians(_transform.getRadians()[i]), _transform.getRotation()[i]);
		}
	}
	if (_transform.getScale().size() > 0) {
		for (auto& _s : _transform.getScale()) {
			m = glm::scale(m, _s);
		}
	}

	//std::cout << isDepth << std::endl;
	
	if (_type == Type::isModel) {	
		if(isDepth)
			DrawModel(m, _material._shadow._depth, asset.getModel(_idAsset), false);
		else
			DrawModel(m, _material._shader, asset.getModel(_idAsset), true);		
	}
	else if(_type == Type::isGeometry) { //Geometry

		_material.setMaterialTextures(asset.getAssetGeometry(_idAsset).getAlbedo(), asset.getAssetGeometry(_idAsset).getSpecular(), asset.getAssetGeometry(_idAsset).getNormal());
		if (isDepth)
			DrawGeometry(m, _material._shadow._depth, asset.getAssetGeometry(_idAsset).getGeometry(), false);
		else
			DrawGeometry(m, _material._shader, asset.getAssetGeometry(_idAsset).getGeometry(), true);
	}
}

void Node::setTrans(Transform& trans) 
{
	//std::cout << trans.getPosition()[0].y << std::endl;
	_transform = trans;
}


void Node::DrawModel(const glm::mat4& transform, const Shader& shader, const Model& model, bool isNormal) {
	shader.set("model", transform);
	//shader.set("view", view);
	//shader.set("proj", proj);
	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}

void Node::DrawGeometry(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, bool isNormal) {

	shader.set("model", transform);
	//shader.set("view", view);
	//shader.set("proj", proj);
	if (isNormal) {
		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}
	geometry.render();
}
