#include "Shape.h"
#include <algorithm>
#include <iostream>
#include "GraphicsCommon.h"

namespace Geometry
{
	Vec2& support(const Vec2 & d, std::vector<Vec2*> vertices)
	{
		float highest = -INFINITY;
		int index = 0;
		for (int i = 0; i < vertices.size(); i++)
		{
			float dot = *vertices[i]*d;
			if (dot > highest)
			{
				highest = dot;
				index = i;
			}
		}
		return *vertices[index];
	}
	Vec2 lineDistance(const Vec2& p, const Vec2& a, const Vec2& b)
	{
		float denom = (b - a).LenSq();
		if (denom < 1e-5f && denom > -1e-5f) return (a+b)/2;
		float u = std::max(0.0f, std::min(1.0f, (p - a)*(b - a) / denom));
		return a*(1.0f - u) + b*u;
	}
	void twoLinesDistance(const Vec2& a, const Vec2& b, const Vec2& p, const Vec2& q, Vec2& outAB, Vec2& outPQ)
	{
		const Vec2* minPoint = &a;
		float minDist = lineDistance(a, p, q).LenSq(),
			  distB   = lineDistance(b, p, q).LenSq(),
			  distP   = lineDistance(p, a, b).LenSq(),
		  	  distQ   = lineDistance(q, a, b).LenSq();
		if (distB < minDist){	minPoint = &b;	minDist = distB;}
		if (distP < minDist){	minPoint = &p;	minDist = distP;}
		if (distQ < minDist){	minPoint = &q;	minDist = distQ;}
		if (minPoint == &a || minPoint == &b)
		{
			outPQ = lineDistance(*minPoint, p, q);
			outAB = lineDistance(outPQ, a, b);
		}
		else if (minPoint == &p || minPoint == &q)
		{
			outAB = lineDistance(*minPoint, a, b);
			outPQ = lineDistance(outAB, p, q);
		}
	}
	Vec2 SMD(const Shape& A, const Shape& B, const Vec2& d) // Support of Minkowski Difference
	{
		return A.support(d) - B.support(-d);
	}
	float sign(const Vec2& p, const Vec2& a, const Vec2& b)
	{
		return (p.x - b.x) * (a.y - b.y) - (a.x - b.x) * (p.y - b.y);
	}
	bool pointInTriangle(const Vec2& p, const Vec2& v1, const Vec2& v2, const Vec2& v3)
	{
		bool b1, b2, b3;

		b1 = sign(p, v1, v2) < 0.0f;
		b2 = sign(p, v2, v3) < 0.0f;
		b3 = sign(p, v3, v1) < 0.0f;

		return ((b1 == b2) && (b2 == b3));
	}
	Vec2 perpendicular(const Vec2& v)
	{
		return Vec2(-v.y, v.x);
	}
	std::vector<Vec2*> findHull(const std::vector<Vec2*>& Sk, Vec2& P, Vec2& Q)
	{
		// Find points on convex hull from the set Sk of points 
		// that are on the right side of the oriented line from P to Q
		if (Sk.size() <= 1) return Sk;
		Vec2 K = perpendicular(Q - P); // perpendicular to line
		Vec2& C = support(K, Sk); // Farthest 1
		//Vec2* F2 = new Vec2(support(-K, Sk)); // Farthest 2
		////From the given set of points in Sk, find farthest point, say C, from segment PQ
		//Vec2* C = (
		//	lineDistance(*F1, P, Q).LenSq()
		//>
		//	lineDistance(*F2, P, Q).LenSq()
		//	) ? F1 : F2;
		//Add point C to convex hull at the location between P and Q
		//Three points P, Q, and C partition the remaining points of Sk into 3 subsets: S0, S1, and S2
		//where S0 are points inside triangle PCQ, S1 are points on the right side of the oriented
		//line from  P to C, and S2 are points on the right side of the oriented line from C to Q.
		
		std::vector<Vec2*> S = { &C };
		std::vector<Vec2*> S1, S2;
		for (Vec2* v : Sk)
		{
			if (sign(*v, P, C) >= 0)
			{
				S1.push_back(v);
			}
			else if (sign(*v, C, Q) >= 0)
			{
				S2.push_back(v);
			}
		}

		S1 = findHull(S1, P, C);
		S2 = findHull(S2, C, Q);
		S.insert(S.end(), S1.begin(), S1.end());
		S.insert(S.end(), S2.begin(), S2.end());
		return S;
	}
	std::vector<Vec2> quickHull(const std::vector<Vec2>& S)
	{
		std::vector<Vec2*> newS, returned;
		std::vector<Vec2> convexHull;
		for (Vec2 v : S){	newS.push_back(new Vec2(v));	}
		returned = quickHull(newS);
		for (Vec2* v : returned){	convexHull.push_back(*v);	}
		for (Vec2* v : newS) { delete v; }
		newS.clear();
		return convexHull;
	}
	std::vector<Vec2*> quickHull(const std::vector<Vec2*>& S)
	{
		Vec2& A = support(Vec2(1, 0), S);
		Vec2& B = support(Vec2(-1, 0), S);
		std::vector<Vec2*> convexHull = { &A, &B };
		std::vector<Vec2*> S1, S2;
		// separate points
		for (Vec2* v : S)
		{
			float s = sign(*v, A, B); // s==0 means that the point is on the line, should be removed
			if (s > 1e-1)
			{
				S1.push_back(v);
			}
			else if(s < -1e-1)
			{
				S2.push_back(v);
			}
		}
		S1 = findHull(S1, A, B);
		S2 = findHull(S2, B, A);
		// only include if it does not exist already
		for (int i = 0; i < S1.size(); i++)
		{
			Vec2* v = S1[i];
			if (std::find(convexHull.begin(), convexHull.end(), v) == convexHull.end()) {
				/* S does not contain v */
				convexHull.push_back(v);
			}
		}
		for (int i = 0; i < S2.size(); i++)
		{
			Vec2* v = S2[i];
			if (std::find(convexHull.begin(), convexHull.end(), v) == convexHull.end()) {
				/* S does not contain v */
				convexHull.push_back(v);
			}
		}
		return convexHull;
	}
	Poly minkowskiDiff(const Poly& A, const Poly& B)
	{
		std::vector<Vec2> allVertices, convexVertices;
		allVertices.reserve(A.vertices.size()*B.vertices.size());
		for (Vec2* a : A.vertices)
		{
			for (Vec2* b : B.vertices)
			{
				allVertices.push_back(Vec2(*a - *b));
			}
		}
		convexVertices = quickHull(allVertices);
		Poly difference(convexVertices);
		difference.sortVertices();
		return difference;
	}
	void closestPoints(const Shape& A, const Shape& B, Vec2& outA, Vec2& outB)
	{
		Vec2 &firstPointA = A.getRandomVertex(),
			 &firstPointB = B.getRandomVertex();
		Vec2 v = firstPointA - firstPointB;
		Simplex W;
		float u = 0;
		bool closeEnough = false;
		while (!closeEnough)
		{
			//Vec2 w = Geometry::SMD(*this, shp, -v);
			Vec2& pointA = A.support(-v);
			Vec2& pointB = B.support(v);
			Vec2 w = pointA - pointB;
			float absV = v.Len();
			float delta = v*w / absV;
			u = std::max(u, delta);
			closeEnough = (absV - u) <= 0.01; // epsilon was tested
			if (closeEnough && W.vertices.size() == 0) // i.e. if first time fill Simplex W
			{
				W.vertices.push_back(w);
				W.verticesA.push_back(&firstPointA);
				W.verticesB.push_back(&firstPointB);
			}
			else if (!closeEnough)
			{
				if (W.vertices.size() < 3)
				{
					W.vertices.push_back(w);
					W.verticesA.push_back(&pointA);
					W.verticesB.push_back(&pointB);
				}
				v = W.conv();
				W.refine();
			}
		}
		Vec2 pointOnA, pointOnB;
		int len = W.vertices.size();
		if (len == 0){	
			return;	}
		if (len == 1)
		{
			W.verticesA.push_back(W.verticesA[0]);
			W.verticesB.push_back(W.verticesB[0]);
		}
		float vLen = v.Len();
		Geometry::twoLinesDistance(*W.verticesA[0], *W.verticesA[1], *W.verticesB[0], *W.verticesB[1], pointOnA, pointOnB);
		//gfx->DrawVector(pointOnA, pointOnB, Colors::Cyan);
		outA = pointOnA;
		outB = pointOnB;
	}
}


Shape::Shape()
{
}


Shape::~Shape()
{
}

Vec2& Poly::support(const Vec2 & d) const
{
	return Geometry::support(d, vertices);
}

Vec2 Poly::center() const
{
	Vec2 total = Vec2(0, 0);
	for (Vec2* v : vertices)
	{
		total += *v;
	}
	return total / vertices.size();
}

float Poly::distance(const Shape & B) const
{
	Vec2 pointOnA, pointOnB;
	Geometry::closestPoints(*this, B, pointOnA, pointOnB);
	return (pointOnB - pointOnA).Len();
}

void Poly::sortVertices()
{
	std::vector<float> angles;
	Vec2 c = center();
	for (Vec2* v : vertices)
	{
		angles.push_back(atan2(v->x - c.x, v->y - c.y));
	}
	size_t length = angles.size();
	for (size_t i = 0; i < length - 1; i++)
	{
		int least = i;
		float minAngle = INFINITY;
		for (size_t j = i; j < length; j++)
		{
			if (angles[j] < minAngle)
			{
				least = j;
				minAngle = angles[j];
			}
		}
		std::swap(angles[i], angles[least]);
		std::swap(vertices[i], vertices[least]);
	}
}

void Poly::move(const Vec2 & pos)
{
	Vec2 c = center();
	for (Vec2* v : vertices)
	{
		*v += pos - c;
	}
}

void Poly::apply(const Mat2 & mat)
{
	for (Vec2* v : vertices)
	{
		*v = *v * mat;
	}
}

void Poly::rotate(float theta)
{
	Vec2 c = center();
	move(-c);
	apply(Mat2::Rotation(theta));
	move(c);
}

Vec2 Simplex::distance(const Vec2 & p)
{
	switch (vertices.size())
	{
	case 1:
		return vertices[0];
		break;
	case 2:
		return Geometry::lineDistance(p, vertices[0], vertices[1]);
		break;
	case 3:
		if (Geometry::pointInTriangle(p, vertices[0], vertices[1], vertices[2]))
		{
			return Vec2(0, 0);
		}
		else
		{
			Vec2 a = Geometry::lineDistance(p, vertices[0], vertices[1]);
			Vec2 b = Geometry::lineDistance(p, vertices[1], vertices[2]);
			Vec2 c = Geometry::lineDistance(p, vertices[2], vertices[0]);
			float distA = (a - p).LenSq();
			float distB = (b - p).LenSq();
			float distC = (c - p).LenSq();
			//std::cout << "distA: " << distA << ", distB: " << distB << ", distC: " << distC << std::endl;
			if (distA <= distB && distA <= distC) return a;
			if (distB <= distC && distB <= distA) return b;
			else return c;
		}
		break;
	}
}

void Simplex::refine()
{
	if (vertices.size() <= 1) return;
	Vec2 v = conv();
	for (int i = vertices.size() - 1; i >= 0; i--)
	{
		Vec2 w = vertices[i];
		vertices.erase(vertices.begin() + i);
		if ((conv() - v).LenSq() > 1e-3)
		{
			vertices.insert(vertices.begin() + i, w);
		}
		else
		{
			verticesA.erase(verticesA.begin() + i);
			verticesB.erase(verticesB.begin() + i);
		}
	}
}
