#ifndef __NODE_H__
#define __NODE_H__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <engine\transform.hpp>
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
	//Material _material;

public:
	Transform _trans;

	enum Type {
		Model,
		Geometry
	};
	Type _type;

	Node(int idAsset, Material& material, Type type);
	//Node(Model& model, Material& material, Type type);
	//Node(Geometry& geometry, Material& material, Type type);

	void setDirtyFlag(bool dirtyFlag);

	void drawNode(const glm::mat4& view, const glm::mat4& proj, Assets& asset);
	void setTrans(Transform& trans);
	Transform getTrans() { return _trans; }
	int getId() { return _idAsset; }
	Material getMaterial() { return _material; }
	//Transform Transform() { return _transform; }
	bool dirtyFlag() { return _dirtyFlag; }
};

#endif