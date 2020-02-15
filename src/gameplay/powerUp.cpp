#include <gameplay/powerUp.hpp>

PowerUp::PowerUp(SceneGraph& sg, Node& node) : _go(sg,node)
{
	Start();
}

PowerUp::~PowerUp()
{
}

void PowerUp::Start()
{
	_used = false;
	_pu_position = glm::vec3(-5.0, 1.0, -5.0);
	_go.setSize(glm::vec3(0.2f,0.2f,0.2f));
}

void PowerUp::Update(const float dt)
{	
	if (_used) {
		_pu_position += glm::vec3(1.0, 0.0, 1.0) * _speed * dt;
		_pu_rotation += _speed * 10.0f * dt;
		PowerUpDraw();
	}
	
}

void PowerUp::setUsed(bool used)
{
	_used = used;
}

void PowerUp::PowerUpDraw()
{
	_go.Init();
	_go.Translate(glm::vec3(_pu_position));
	//_go.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Rotate(_pu_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	_go.Scale(glm::vec3(0.4f, 0.4f, 0.4f));
	_go.readyToDraw();
}
