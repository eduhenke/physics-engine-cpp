#pragma once
#include "Shape.h"
#include <iostream>
#include "Configs.h"

#define FRICTION_FACTOR 1

class Object
{
public:
	float mass = 1.0f;
	Vec2     accel = nullVec, veloc = nullVec;
	float angAccel = 0.0f, angVeloc = 0.0f;
	Shape& shape;
	Object(Shape& shp) : shape(shp) {};
	~Object();
	void handleCollision(Object &const B);
	void move()
	{
		veloc += accel;
		shape.move(veloc);
		angVeloc += angAccel;
		shape.rotate(angVeloc/10000.0f);
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
