/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "ChiliMath.h"

template <typename T>
class _Vec2
{
public:
	_Vec2()
	{}
	_Vec2( T x,T y )
		:
		x( x ),
		y( y )
	{}
	_Vec2( const _Vec2& vect )
		:
		_Vec2( vect.x,vect.y )
	{}
	_Vec2(const std::pair<T, T>& pair)
		:
		_Vec2(pair.first, pair.second)
	{}
	template <typename T2>
	explicit operator _Vec2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		return sq( *this );
	}
	T		Len() const
	{
		return sqrt( LenSq() );
	}
	_Vec2&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	_Vec2	GetNormalized() const
	{
		_Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}
	_Vec2	operator-() const
	{
		return _Vec2( -x,-y );
	}
	_Vec2&	operator=( const _Vec2 &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	_Vec2&	operator+=( const _Vec2 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	_Vec2&	operator-=( const _Vec2 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T		operator*( const _Vec2 &rhs ) const
	{
		return x * rhs.x + y * rhs.y;
	}
	_Vec2	operator+( const _Vec2 &rhs ) const
	{
		return _Vec2( *this ) += rhs;
	}
	_Vec2	operator-( const _Vec2 &rhs ) const
	{
		return _Vec2( *this ) -= rhs;
	}
	_Vec2&	operator*=( const T &rhs )
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	_Vec2	operator*( const T &rhs ) const
	{
		return _Vec2( *this ) *= rhs;
	}
	_Vec2&	operator/=( const T &rhs )
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	_Vec2	operator/( const T &rhs ) const
	{
		return _Vec2( *this ) /= rhs;
	}
	bool	operator==( const _Vec2 &rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=( const _Vec2 &rhs ) const
	{
		return !(*this == rhs);
	}
	_Vec2	InterpolateTo( const _Vec2& dest,T alpha ) const
	{
		return *this + (dest - *this) * alpha;
	}
	_Vec2 Rotate( double degrees) const
	{
		float c = cos(degrees), s = sin(degrees);
		return _Vec2((x*c) - (y*s), (x*s) + (y*c));
	}
	_Vec2 Clamp(const T lowerX, const T lowerY, const T upperX, const T upperY)
	{
		return _Vec2(clamp(x, lowerX, upperX), clamp(y, lowerY, upperY));
	}
	bool Almost( const _Vec2& rhs ) const
	{
		return Almost(rhs, 1e-3);
	}
	bool Almost(const _Vec2& rhs, T eps) const
	{
		return abs(x - rhs.x) < eps && (y - rhs.y) < eps;
	}
	void Print() const
	{
		std::cout << "( " << x << ", " << y << " )" << std::endl;
	}

	float Cross(const _Vec2& rhs) const // cross product analog
	{
		return x*rhs.y - rhs.x*y;
	}
public:
	T x;
	T y;
};

typedef _Vec2<float> Vec2;
typedef _Vec2<double> Ved2;
typedef _Vec2<int> Vei2;

extern Vec2 nullVec;