#include <engine\node.hpp>
#include <iostream>
#include <glm\ext\matrix_transform.hpp>


Node::Node(int idAsset,Material& material, Type type) : _idAsset(idAsset), _material(material), _type(type)  {

}


void Node::setDirtyFlag(bool dirtyFlag) {
	_dirtyFlag = dirtyFlag;
}

glm::mat4 Node::matFromTransform(Transform t)
{
	glm::mat4 m = glm::mat4(1.0);

	if (t.getPosition().size() > 0) {
		for (auto& _t : t.getPosition()) {
			m = glm::translate(m, _t);
		}
	}

	if (t.getRotation().size() > 0) {
		for (int i = 0; i < t.getRotation().size(); ++i) {
			m = glm::rotate(m, glm::radians(t.getRadians()[i]), t.getRotation()[i]);
		}
	}
	if (t.getScale().size() > 0) {
		for (auto& _s : t.getScale()) {
			m = glm::scale(m, _s);
		}
	}

	return m;
}

void Node::drawNode(Assets& asset, bool isDepth)
{

	//First we get the values of class Transform and we will convert it into a mat4 in the right order position/rotation/translate
	glm::mat4 m = glm::mat4(1.0f);
	if (_hasParent) {
		//Tw(i) = Tw(parent)*Tl
		glm::mat4 mp = matFromTransform(_parent);
		glm::mat4 ml = matFromTransform(_transform);
		m = mp * ml;
	}
	else {
		m = matFromTransform(_transform);
	}
		

	//If it is Typpe model -> draw model
	if (_type == Type::isModel) {	
		if (isDepth) { //<- First pass
			DrawModel(m, _material.getShadow()._depth, asset.getModel(_idAsset), false);
		}
		else { //<- Second pass
			DrawModel(m, _material._shader, asset.getModel(_idAsset), true);
		}
			
	}
	//If it is Geometry model -> draw Geometry
	else if(_type == Type::isGeometry) {

		if (isDepth) { //<- First pass
			DrawGeometry(m, _material.getShadow()._depth, asset.getAssetGeometry(_idAsset).getGeometry(), false);
		}
			
		else {  //<- Second pass
			_material.setMaterialTextures(asset.getAssetGeometry(_idAsset).getAlbedo(), asset.getAssetGeometry(_idAsset).getSpecular(), asset.getAssetGeometry(_idAsset).getNormal());
			DrawGeometry(m, _material._shader, asset.getAssetGeometry(_idAsset).getGeometry(), true);
		}
			
	}
}

void Node::setTrans(Transform& trans) 
{
	_transform = trans;
}

void Node::setParent(int id, Transform t)
{
	_hasParent = true;
	_parentID = id;
	_parent = t;
}


void Node::DrawModel(const glm::mat4& transform, const Shader& shader, const Model& model, bool isNormal) {
	shader.set("model", transform);
	if (isNormal) {
		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}
	model.render(shader);
}

void Node::DrawGeometry(glm::mat4& transform, const Shader& shader, const Geometry& geometry, bool isNormal) {
	shader.set("model", transform);
    if (isNormal) {
		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(transform)));
		shader.set("normalMat", normalMat);
	}
	geometry.render();
}
