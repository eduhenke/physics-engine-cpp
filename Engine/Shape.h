#pragma once
#include "Mat2.h"
#include <vector>
#include <algorithm>
#include "GraphicsCommon.h"

class Shape
{
public:
	Shape();
	~Shape();
	virtual void goTo(const Vec2& pos) { goTo(pos.x, pos.y); };
	virtual void goTo(float x, float y) {};
	virtual void move(const Vec2& pos) { move(pos.x, pos.y); };
	virtual void move(float dx, float dy) {};
	virtual void rotate(float theta)   {};

	virtual Vec2 center()				 const  { return nullVec; };
	virtual Vec2 support(const Vec2& d)  const	{ return nullVec; };
	virtual Vec2 getRandomVertex()		 const	{ return nullVec; };
	virtual void draw() const {};
};

class Simplex
{
public:
	std::vector<Vec2> vertices;

	// vertices of A and B that created this vertex
	std::vector<Vec2> verticesA;
	std::vector<Vec2> verticesB;
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
	Poly(std::initializer_list<Vec2> l) : Poly(std::vector<Vec2>(l)){};
	Poly(std::initializer_list<float> l)
		//0.0, 0.3, 0.4, 0.5
	{
		int i = 0;
		float first, second;
		for (float num : l)
		{
			if (i % 2 == 0)
			{
				first = num;
			}
			else if (i % 2 == 1)
			{
				second = num;
				vertices.push_back(new Vec2(first, second));
			}
			i++;
		}
	};
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
	// for constructing rectangles
	Poly(float cx, float cy, float w, float h) :
		Poly({
		Vec2(cx - w/2, cy - h/2),
		Vec2(cx - w/2, cy + h/2),
		Vec2(cx + w/2, cy + h/2),
		Vec2(cx + w/2, cy - h/2),
	}) {};
	~Poly()
	{
		for (std::vector< Vec2* >::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			delete (*it);
		}
		vertices.clear();
	}
	Vec2 getRandomVertex() const
	{
		return *vertices[0];
	};
	Vec2 support(const Vec2& d) const;
	Vec2 center() const;
	float distance(const Shape& B) const;
	void sortVertices();
	void goTo(const Vec2& pos) { goTo(pos.x, pos.y); };
	void goTo(float x, float y);
	void move(const Vec2& pos) { move(pos.x, pos.y); };
	void move(float dx, float dy);
	void apply(const Mat2& mat);
	void rotate(float theta);

	void draw() const
	{
		std::vector<Vec2> drawingVertices;
		for (Vec2* v : vertices)
		{
			drawingVertices.push_back(Vec2(*v));
		}
		for (Vec2& v : drawingVertices)
		{
			v.x += WIDTH / 2;
			v.y = HEIGHT / 2 - v.y;
		}
		gfx->DrawPolygon(drawingVertices, Colors::White); 
	};
};

class Circle : public Shape
{
public:
	float r;
	Vec2 pos;
	Circle(const Vec2& pos, float r) : pos(pos), r(r) {};
	Vec2 center() const { return pos; };
	Vec2 getRandomVertex() const { return Vec2(pos.x + r, pos.y); };
	Vec2 support(const Vec2& d) const { return pos + d.GetNormalized()*r; };
	void draw() const { gfx->DrawCircle(WIDTH / 2 + pos.x, HEIGHT / 2 - pos.y, r, Colors::Yellow); };
	void goTo(const Vec2& pos) { goTo(pos.x, pos.y); };
	void goTo(float x, float y) { pos.x = x; pos.y = y; };
	void move(const Vec2& pos) { move(pos.x, pos.y); };
	void move(float dx, float dy) { pos.x += dx; pos.y += dy; };
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
