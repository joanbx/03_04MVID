#include "..\..\include\gameplay\floor.hpp"

Floor::Floor(std::vector<GameObject>& floors) : _floors(floors)
{

}

Floor::~Floor()
{
}

void Floor::Start()
{
}

void Floor::Update(const float dt)
{
	int count = 0;
	if (movement - count * _scale >= _scale * (_floors.size()-1)) { movement = 0; }
	else{ movement += speed * dt; }

	for (auto& floor : _floors) {
		floor.Init();
		floor.Translate(glm::vec3(0.0f, -0.5f, movement - count* _scale));
		floor.Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		floor.Scale(glm::vec3(_scale, _scale, _scale));
		floor.readyToDraw();
		count++;
	}
}
