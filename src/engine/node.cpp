#include <engine\node.hpp>
#include <iostream>
#include <glm\ext\matrix_transform.hpp>


Node::Node(int idAsset,Material& material, Type type) : _idAsset(idAsset), _material(material), _type(type) {

}


void Node::setDirtyFlag(bool dirtyFlag) {
	_dirtyFlag = dirtyFlag;
}



void Node::drawNode(const glm::mat4& view, const glm::mat4& proj, Assets& asset)
{

	//Do Transform

	glm::mat4 m = glm::mat4(1.0);
	std::cout << _trans.getPosition()[0].y << std::endl;
	/*std::cout << _trans.getPosition().size() << std::endl;
	std::cout << _trans.getRotation().size() << std::endl;
	std::cout << _trans.getRadians().size() << std::endl;
	std::cout << _trans.getScale().size() << std::endl;*/
	
	if (_trans.getPosition().size() > 0) {
		for (auto& _t : _trans.getPosition()) {
			m = glm::translate(m, _t);
		}
	}
	
	if (_trans.getRotation().size() > 0) {
		for (int i = 0; i < _trans.getRotation().size(); ++i) {
			m = glm::rotate(m, glm::radians(_trans.getRadians()[i]), _trans.getRotation()[i]);
		}
	}
	if (_trans.getScale().size() > 0) {
		for (auto& _s : _trans.getScale()) {
			m = glm::scale(m, _s);
		}
	}
	
	if (_type == Type::Model) {

			std::cout << "DRAW" << " " << _material.getName() <<std::endl;
			
			Draw(m, _material._shader, asset.getModel(_idAsset), view, proj, true);
			//Draw(_transform.transform, view, proj, asset.getModel(_idAsset), _material);
			
			/*_material._shader.use();
			_material._shader.set("viewPos", glm::vec3(0.0f, 10.0f, 0.0f));
			_material.setMaterialLights();
			_material._shader.set("model", _trans);
			_material._shader.set("view", view);
			_material._shader.set("proj", proj);

			glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(_trans)));
			_material._shader.set("normalMat", normalMat);*/

			//asset.getModel(_idAsset).render(_material._shader);
		
	}
	else if(_type == Type::Geometry) { //Geometry
		//std::cout << _idAsset << std::endl;
		//if (asset.getAssetGeometry(_idAsset)._textures.size() > 0) {
			//std::cout << asset.getAssetGeometry(_idAsset)._textures.size() << std::endl;
		//}
		
		/*if (asset.getAssetGeometry(_idAsset)._textures.size() == 3)
			_material.setMaterialTextures(asset.getAssetGeometry(_idAsset)._textures[0], asset.getAssetGeometry(_idAsset)._textures[1], asset.getAssetGeometry(_idAsset)._textures[2]);
		
		_material._shader.use();
		_material._shader.set("viewPos", glm::vec3(0.0f, 10.0f, 0.0f));
		_material.setMaterialLights();
		_material._shader.set("model", _trans);
		_material._shader.set("view", view);
		_material._shader.set("proj", proj);

		glm::mat4 normalMat = glm::inverse(glm::transpose(glm::mat3(_trans)));
		_material._shader.set("normalMat", normalMat);
		asset.getAssetGeometry(_idAsset)._geometry.render();*/
		
		//Draw(_trans, _material._shader, asset.getAssetGeometry(_idAsset), view, proj, true);
		//Draw(_trans, _material._shader, asset.getAssetGeometry(_idAsset), view, proj, const Texture & t_albedo, const Texture & t_specular, const Texture & t_normal);

		//Draw(const glm::mat4 & transform, const Shader & shader, const Geometry & geometry, const glm::mat4 & view, const glm::mat4 & proj, bool isNormal);
		//Draw(const glm::mat4 & transform, const Shader & shader, const Geometry & geometry, const glm::mat4 & view, const glm::mat4 & proj, const Texture & t_albedo, const Texture & t_specular, const Texture & t_normal);


	}
}

void Node::setTrans(Transform& trans)
{
	_trans = trans;
}
