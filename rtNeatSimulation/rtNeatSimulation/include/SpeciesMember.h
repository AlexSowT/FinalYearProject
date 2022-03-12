#pragma once
#include <memory>
#include "GameObject.h"

class SpeciesMember : public GameObject
{
public:
	SpeciesMember();

	void Update(float deltaTime);

	void SetSpeed(float speed) { m_speed = speed; }
private:
	float m_speed = 0.f;
};