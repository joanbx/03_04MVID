#include "..\..\include\gameplay\asteroid.hpp"

Asteroid::Asteroid(SceneGraph& sg, Node& node) : _go(sg,node)
{
	Start();
}

void Asteroid::Start()
{
	glm::vec3 size = glm::vec3(0.5f);

	_go.setSize(size);
	_used = true;
	_inFrustum = false;
	//initial pos
	glm::vec2 cambounds = _go.camBounds();
	float x = -cambounds.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * cambounds.x)));
	_posAsteroid= glm::vec3(x, _go.Position().y, -cambounds.y - size.y);
	if (x < 0) {
		_modPos = glm::vec3(0.4f, 0, 1.0f);
	}
	else {
		_modPos = glm::vec3(-0.4f, 0, 1.0f);
	}
	_go.Translate(_posAsteroid);
}

void Asteroid::Update(float dt)
{
	
	if (_used) {
		_posAsteroid += _modPos * _speedTranslation * dt;
		_angleAsteroid += _speedRotation * dt;
		if (_angleAsteroid > 360) _angleAsteroid = 0;
		//std::cout << _angleAsteroid << std::endl;
		asteroidDraw();
		
		if (_go.in_frustum() == false && _inFrustum) {
			Start();
		}
		else if (!_inFrustum && _go.in_frustum() == true) {
			_inFrustum = true;
		}
	}
	
}


void Asteroid::setUse(bool use)
{
	_used = use;
}


void Asteroid::asteroidDraw()
{
	_go.Init();
	_go.Translate(_posAsteroid);
	_go.Rotate(_angleAsteroid, glm::vec3(0.0f, 1.0f, 0.0f));
	_go.Scale(glm::vec3(0.005f));
	_go.readyToDraw();
}