#include <gameplay\bullet.hpp>


Bullet::Bullet(SceneGraph& sg, Node& node, Bullet::Bullettypes type) : _go(sg,node), _type(type){

	_go.setSize(glm::vec3(0.1f));

}

Bullet::~Bullet()
{
}

void Bullet::Start() {
	_used = false;
	_posBullet = glm::vec3(0, 1, 0);
}

void Bullet::Update(const float dt) {
	if (_used) {
		pushDirection(dt);
		bulletDraw();
		checkAutoDestroy();
	}
		
}
 
void Bullet::setUse(bool used) {
	_used = used;
}

void Bullet::setStarted(bool start)
{
	_started = start;
}

void Bullet::setUpdated(bool updated)
{
	_updated = updated;
}

void Bullet::setPosition(glm::vec3& posBullet) {
	_posBullet = posBullet;
}

void Bullet::setAngle(float angle)
{
	_angleBullet = angle;
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


void Bullet::setDirection(glm::vec3 direction) { _direction = direction; }

void Bullet::setSpeed(float speed)
{
	_speed = speed;
}

void Bullet::bulletDraw() {
	//std::cout << "Bullet Draw" << std::endl;
	_go.Init();
	_go.Translate(_posBullet);
	_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Rotate(_angleBullet, glm::vec3(0.0f, 0.0f, 1.0f));
	_go.Scale(glm::vec3(2.3f,2.3f, 2.3f));
	_go.readyToDraw();
}