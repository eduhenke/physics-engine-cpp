#pragma once
#include "Shape.h"
#include <iostream>
#include "Configs.h"

#define FRICTION_FACTOR 1

class Object
{
public:
	float mass = 1.0f;
	Vec2 accel = nullVec, veloc = nullVec;
	Shape& shape;
	Object(Shape& shp) : shape(shp) {};
	~Object();
	void handleCollision(Object &const B);
	void move()
	{
		veloc = veloc*FRICTION_FACTOR + accel;
		shape.move(veloc);
	}
	Vec2 momentum()
	{
		return veloc*mass;
	}
};


namespace Physics
{
	extern Vec2 VelocityAfterCollision(const Object& A, const Object& B, const Vec2& N);
}
