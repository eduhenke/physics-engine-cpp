#pragma once
#include "Shape.h"
#include <iostream>
#include "Configs.h"

#define FRICTION_FACTOR 0.9999
#define GRAVITY Vec2(0.0f,-980.f)

class Object
{
public:
	bool movable = true;
	float mass = 1.0f;
	Vec2     accel = GRAVITY, veloc = nullVec;
	float angAccel = 0.0f, angVeloc = 0.0f;
	float momentOfInertia = 2e2;
	Shape& shape;
	Object(Shape& shp) : shape(shp) {};
	~Object();
	void handleCollision(Object &const B, float dt);
	void move(float dt)
	{
		if (movable)
		{
			veloc *= FRICTION_FACTOR;
			veloc += accel*dt;
			shape.move(veloc*dt);
			angVeloc += angAccel*dt;
			shape.rotate(angVeloc*dt);
		}
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
