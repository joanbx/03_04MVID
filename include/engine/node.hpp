#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
#include <engine\model.hpp>
#include <engine\material.hpp>
#include <engine\assets.hpp>
//Class Node: Nodes that hangs from the scnegraph
//It has also a drawable function
class Node {

private:
	
	bool _dirtyFlag = false;
	
	int _idAsset;
	Material& _material;
	Transform _transform;
	bool _hasParent = false;
	Transform _parent;
	int _parentID;
	
	glm::mat4 matFromTransform(Transform t);

public:
	//Type: Geometry od Model
	enum Type {
		isModel,
		isGeometry
	};
	
	Type _type;
	
	//Construrctor: Needs id-asset (from class asset), material, and type (geometry or model)
	Node(int idAsset, Material& material, Type type);

	//drawNode: Drawable function. Needs asset container
	void drawNode(Assets& asset, bool isDepth);
	//DrawModel: Drawable function for models
	void DrawModel(const glm::mat4& transform, const Shader& shader, const Model& model, bool isNormal);
	//DrawGeometry: Drawable function for geometries
	void DrawGeometry(glm::mat4& transform, const Shader& shader, const Geometry& geometry, bool isNormal);

	//setDirtyFlag: Set Dirty Flag to know if it has been already drawn
	void setDirtyFlag(bool dirtyFlag);
	//setTrans: Set transform value
	void setTrans(Transform& trans);
	//setParent: Set Parent node
	void setParent(int id, Transform t);

	//getTrans: get transform
	Transform getTrans() { return _transform; }
	//getId: get asset id
	int getId() { return _idAsset; }
	//getMaterial: get material linked to this object
	Material getMaterial() { return _material; }
	//dirtyFlag: returns value dirty flag
	bool dirtyFlag() { return _dirtyFlag; }
	//getParent: get Parent transform
	int getParent() { return _parentID; }
	//hasParent: returns if the node has a parent
	bool hasParent() { return _hasParent; }
};

#endif