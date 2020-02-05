#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
#include <engine\model.hpp>
#include <engine\Material.hpp>
#include <engine\Drawable.hpp>
#include <engine\Assets.hpp>
#include <engine\model.hpp>

class Node {

	

private:

	//Transform _transform;
	
	bool _dirtyFlag = false;
	
	int _idAsset;
	Material& _material;
	Transform _transform;
	//Material _material;

public:
	
	enum Type {
		isModel,
		isGeometry
	};
	
	Type _type;

	Node(int idAsset, Material& material, Type type);
	//Node(Model& model, Material& material, Type type);
	//Node(Geometry& geometry, Material& material, Type type);

	void setDirtyFlag(bool dirtyFlag);

	void drawNode(Assets& asset, bool isDepth);

	void DrawModel(const glm::mat4& transform, const Shader& shader, const Model& model, bool isNormal);
	void DrawGeometry(const glm::mat4& transform, const Shader& shader, const Geometry& geometry, bool isNormal);


	void setTrans(Transform& trans);
	Transform getTrans() { return _transform; }
	int getId() { return _idAsset; }
	Material getMaterial() { return _material; }
	//Transform Transform() { return _transform; }
	bool dirtyFlag() { return _dirtyFlag; }
};

#endif