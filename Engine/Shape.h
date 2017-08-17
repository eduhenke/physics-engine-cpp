#pragma once
#include "Mat2.h"
#include <vector>
#include <algorithm>

class Shape
{
public:
	Shape();
	~Shape();
	virtual Vec2& support(const Vec2& d) const
	{
		//Vec2* v = new Vec2(0, 0);
		return nullVec;
	};
	virtual Vec2& getRandomVertex() const
	{
		//Vec2* v = new Vec2(0, 0);
		return nullVec;
	};
};

class Simplex
{
public:
	std::vector<Vec2> vertices;

	// vertices of A and B that created this vertex
	std::vector<Vec2*> verticesA;
	std::vector<Vec2*> verticesB;
	Simplex() {};
	Simplex(std::vector<Vec2>& v) : vertices(v) {};
	Vec2 conv()
	{
		return distance(Vec2(0, 0));
	}
	Vec2 distance(const Vec2& p);
	void refine();
};

class Poly : public Shape
{
public:
	std::vector<Vec2*> vertices;
	Poly() {};
	Poly(const Poly& p) 
	{
		vertices.reserve(p.vertices.size());
		for (Vec2* vec : p.vertices)
		{
			vertices.push_back(new Vec2(*vec));
		}

	};
	Poly(const std::vector<Vec2*>& v) : vertices(v)	{};
	Poly(const std::vector<Vec2>& v)
	{
		vertices.reserve(v.size());
		for (Vec2 vec : v)
		{
			vertices.push_back(new Vec2(vec));
		}
	};
	~Poly()
	{
		for (std::vector< Vec2* >::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			delete (*it);
		}
		vertices.clear();
	}
	Vec2& getRandomVertex() const
	{
		return *vertices[0];
	};
	Vec2& support(const Vec2& d) const;
	Vec2 center() const;
	float distance(const Shape& B) const;
	void sortVertices();
	void move(const Vec2& pos);
	void apply(const Mat2& mat);
	void rotate(float theta);
};

namespace Geometry
{
	extern Vec2 lineDistance(const Vec2& p, const Vec2& a, const Vec2& b);
	extern void twoLinesDistance(const Vec2& a, const Vec2& b, const Vec2& p, const Vec2& q, Vec2& outAB, Vec2& outPQ);
	extern Vec2& support(const Vec2 & d, std::vector<Vec2*> vertices);
	extern Poly minkowskiDiff(const Poly& A, const Poly& B);
	extern Vec2 SMD(const Shape& A, const Shape& B, const Vec2& d);	// Support of Minkowski Difference
	extern float sign(const Vec2& p, const Vec2& a, const Vec2& b);
	extern bool pointInTriangle(const Vec2& p, const Vec2& v1, const Vec2& v2, const Vec2& v3);
	extern Vec2 perpendicular(const Vec2& v);
	extern std::vector<Vec2*> findHull(const std::vector<Vec2*>& Sk, Vec2& P, Vec2& Q);
	extern std::vector<Vec2*> quickHull(const std::vector<Vec2*>& S);
	extern void closestPoints(const Shape& A, const Shape& B, Vec2& outA, Vec2& outB);

}
