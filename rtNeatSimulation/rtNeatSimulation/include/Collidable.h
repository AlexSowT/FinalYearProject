#pragma once
#include<memory>

enum COLLIDABLE_TAG{
	TRIGGER, 
	SOLID,
	ENTITY
};

class Collidable {
public:
	Collidable(COLLIDABLE_TAG tag) : tag(tag) {}
	~Collidable();

	virtual void OnCollision(std::shared_ptr<Collidable> other) = 0;
	virtual bool CheckCollision(std::shared_ptr<Collidable> other) = 0;

private:
	COLLIDABLE_TAG tag;
};