#include <gameplay\bullet.hpp>


Bullet::Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes type) : _go(sg,node), _type(type){

	_go.setSize(glm::vec3(0.1f));

}

void Bullet::Start() {

}

void Bullet::Update(float dt) {
	if (_used) {
		pushDirection(dt);
		bulletDraw();
		checkAutoDestroy();
	}
		
}
 
void Bullet::setUse(bool used) {
	_used = used;
}

void Bullet::setPosition(glm::vec3& posBullet) {
	_posBullet = posBullet;
}

void Bullet::pushDirection(float dt) {
	//glm::vec3 forwardBullet(0.0f, 0.0f, -1.0f);
	_posBullet += _direction * _speed * dt;
}

void Bullet::checkAutoDestroy() {
	//std::cout << "bullet " << _go.Position().z << std::endl;
	if (_go.in_frustum(glm::vec3(0,0,0)) == false) {	
		_used = false;
	}
}

bool Bullet::CheckCollisionXZ(GameObject go) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = (_go.Position().x + _go.getSize().x >= go.Position().x &&
		go.Position().x + go.getSize().x >= _go.Position().x) ||
		(_go.Position().x - _go.getSize().x <= go.Position().x &&
			go.Position().x - go.getSize().x <= _go.Position().x);
	// Collision y-axis?
	bool collisionZ = (_go.Position().z + _go.getSize().z >= go.Position().z &&
		go.Position().z + go.getSize().z >= _go.Position().z) ||
		(_go.Position().z - _go.getSize().z <= go.Position().z &&
			go.Position().z - go.getSize().z <= _go.Position().z);
	// Collision only if on both axes


	//std::cout << collisionX << " " << collisionZ << std::endl;
	//std::cout << collisionX << " " << _go.Transform().getPosition().x << " " << go.Transform().getPosition().x << std::endl;

	return collisionX && collisionZ;
}

void Bullet::setDirection(glm::vec3 direction) { _direction = direction; }

void Bullet::bulletDraw() {
	//std::cout << "Bullet Draw" << std::endl;
	_go.Init();
	_go.Translate(_posBullet);
	_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//_go.Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	_go.Scale(glm::vec3(0.0025f));
	_go.readyToDraw();
}