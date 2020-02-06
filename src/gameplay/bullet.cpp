#include <gameplay\bullet.hpp>


Bullet::Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes type) : _go(sg,node), _type(type){

	//switch (type) {
	//	case Bullet::Bullettypes::Cubic: 
	//		//Create New Cube
	//}

}

void Bullet::Start() {

}

void Bullet::Update(float dt) {
	bulletDraw();
}

void Bullet::bulletDraw() {
	_go.Init();
	_go.Translate(posBullet);
	_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	if (angleBullet != 0.0f) _go.Rotate(angleBullet, rotBullet);
	_go.Scale(glm::vec3(0.001f, 0.001f, 0.001f));
	_go.readyToDraw();
}