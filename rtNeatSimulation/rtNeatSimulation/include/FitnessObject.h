#pragma once
#include "Collidable.h"
#include <glm.hpp>

class FitnessObject : Collidable{
public:
	FitnessObject(float width, float heigth, glm::vec2 positon) : Collidable(COLLIDABLE_TAG::TRIGGER), width(width), heigth(heigth), position(position) {}
	~FitnessObject();
	
	void onCollision(Collidable* other) override;

private:
	float width;
	float heigth;
	glm::vec2 position;
	int priority; // TODO: An idea to experiment with, making some areas more important. 
};
