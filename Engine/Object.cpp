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

void Object::handleCollision(Object & const B)
{
	Object &const A = *this;
	Vec2 closestA, closestB, N, distVec;
	float dist;
	Geometry::closestPoints(A.shape, B.shape, closestA, closestB);
	distVec = closestA - closestB;
	dist = distVec.Len();
	N = distVec / dist; // inward, B to A
	A.angAccel = 0.0f;
	B.angAccel = 0.0f;
	if (dist < std::max(10.0f, (A.veloc-B.veloc).Len()))
	{
		// to avoid one shape being "stuck" inside the other
		A.shape.move(-A.veloc);
		B.shape.move(-B.veloc);

		// solve center of mass velocity
		Vec2 Va = Physics::VelocityAfterCollision(A, B, N);
		B.veloc = Physics::VelocityAfterCollision(B, A, -N);
		A.veloc = Va;

		// solve rotation
		Vec2 F = N; // assuming the force is the normal
		Vec2 Ra = closestA - A.shape.center();
		float torque = Ra.Cross(F);
		A.angAccel = torque; // missing momentum of inertia
		Vec2 Rb = closestB - B.shape.center();
		torque = Rb.Cross(-F);
		B.angAccel = torque; // missing momentum of inertia
	}
#if COLLISION_DEBUG
	Vec2 Ca = A.shape.center(), Cb = B.shape.center();
	gfx->DrawVector(A.veloc * 50 + Ca, Ca, Colors::Red);
	gfx->DrawVector(B.veloc * 50 + Cb, Cb, Colors::Red);
	gfx->DrawVector(closestA, closestB, Colors::Gray);
#endif
	A.move();
	B.move();

}