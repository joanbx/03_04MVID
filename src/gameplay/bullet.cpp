#include <gameplay\bullet.hpp>


Bullet::Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes type) : _go(sg,node), _type(type){



}

void Bullet::Start() {

}

void Bullet::Update(float dt) {
	if (_used) {
		pushForward();
		bulletDraw();
		checkDestroy();
	}
		
}
 
void Bullet::setUse(bool used) {
	_used = used;
}

void Bullet::setPosition(glm::vec3& posBullet) {
	_posBullet = posBullet;
}

void Bullet::pushForward() {
	glm::vec3 forwardBullet(0.0f, 0.0f, -1.0f);
	_posBullet += forwardBullet * _speed;
}

void Bullet::checkDestroy() {
	if (_go.in_frustum() == false) {
		_used = false;
	}
}

void Bullet::bulletDraw() {
	//std::cout << "Bullet Draw" << std::endl;
	_go.Init();
	_go.Translate(_posBullet);
	_go.Scale(glm::vec3(0.25f));
	_go.readyToDraw();
}