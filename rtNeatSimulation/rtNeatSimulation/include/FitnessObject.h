#pragma once
#include "Collidable.h"
#include <glm.hpp>
#include <memory>

class FitnessObject : Collidable{
public:
	FitnessObject(float width, float heigth, glm::vec2 positon) : Collidable(COLLIDABLE_TAG::TRIGGER), width(width), heigth(heigth), position(position) {}
	~FitnessObject();
	
	void OnCollision(std::shared_ptr<Collidable> other) override;
	bool CheckCollision(std::shared_ptr<Collidable> other) override;

private:
	float width;
	float heigth;
	glm::vec2 position;
	int priority; // TODO: An idea to experiment with, making some areas more important. 
};
