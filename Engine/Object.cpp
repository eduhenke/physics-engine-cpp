#include "Object.h"

namespace Physics
{
	Vec2 VelocityAfterCollision(const Object& A, const Object& B, const Vec2& N)
	{
		return A.veloc - N * ((2 * B.mass) / (A.mass + B.mass)) * (N*(A.veloc - B.veloc));
	}
}

Object::~Object()
{
}

void Object::handleCollision(Object & const B, float dt)
{
	Object &const A = *this;
	Vec2 closestA, closestB, N, distVec;
	float dist;
	Geometry::closestPoints(A.shape, B.shape, closestA, closestB);
	distVec = closestA - closestB;
	dist = distVec.Len();
	N = distVec / dist; // inward, B to A
	Vec2 dx = (A.veloc - B.veloc) * dt;
	float triggerDist = std::max(6.0f, dx.Len());
	if (dist < triggerDist)
	{
		A.accel = nullVec;
		B.accel = nullVec;
		// to avoid one shape being "stuck" inside the other
		if (A.movable) A.shape.move(N * triggerDist);
		if (B.movable) B.shape.move(-N * triggerDist);

		// solve center of mass velocity
		Vec2 Va = Physics::VelocityAfterCollision(A, B, N);
		B.veloc = Physics::VelocityAfterCollision(B, A, -N);
		A.veloc = Va;

		// solve rotation
		Vec2 F = N*(A.veloc - B.veloc).Len(); // assuming the force is the normal
		Vec2 r = closestA - A.shape.center();
		float torque = r.Cross(F);
		A.angAccel = torque / A.momentOfInertia;
		r = closestB - B.shape.center();
		torque = r.Cross(-F);
		B.angAccel = torque / B.momentOfInertia;
	}
#if COLLISION_DEBUG
	if (dist < 100)
	{
		gfx->DrawVector(closestA, closestB, Colors::Cyan);
	}
#endif

}