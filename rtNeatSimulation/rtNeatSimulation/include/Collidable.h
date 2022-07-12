#pragma once

enum COLLIDABLE_TAG{
	TRIGGER, 
	SOLID,
	ENTITY
};

class Collidable {
public:
	Collidable(COLLIDABLE_TAG tag) : tag(tag) {}
	virtual ~Collidable();

	virtual void onCollision(Collidable* other) = 0;

private:
	COLLIDABLE_TAG tag;
};