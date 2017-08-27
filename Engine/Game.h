/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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

#include "Mat3.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Shape.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "Object.h"
#include "GraphicsCommon.h"
#include <time.h>

using namespace std;

class Game
{
public:
	Game(class MainWindow& w);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	/********************************/
	/*  User Variables              */
	/********************************/
	Poly p1 = { Vec2(110,120), Vec2(160,120), Vec2(170,180), Vec2(130,200), Vec2(100, 140) };
	Poly p2 { 100.0f, 0.0f,   200.0f, 100.0f,   0.0f, 100.0f };
	//Poly floorShape = { Vec2(-WIDTH / 2,-HEIGHT / 2 + 20), Vec2(WIDTH / 2,-HEIGHT / 2 + 20), Vec2(WIDTH / 2,-HEIGHT / 2 - 20), Vec2(-WIDTH / 2,-HEIGHT / 2 - 20) };
	Poly floorShape = Poly(0.0f, -HEIGHT/2, WIDTH, 40.0f);
	Poly lWallShape = Poly(-WIDTH / 2, 0, 40.0f, HEIGHT);
	Poly rWallShape = Poly(WIDTH / 2, 0, 40.0f, HEIGHT);
	Poly roofShape = Poly(0.0f, HEIGHT / 2, WIDTH, 40.0f);
	Circle c1 = Circle(Vec2(-200, -150), 30);
	Object obj1 = Object(p1), obj2 = Object(p2), floor = Object(floorShape, false),
		   rWall = Object(rWallShape, false), lWall = Object(lWallShape, false), roof = Object(roofShape, false);
	vector<Object*> scene{ &obj1, &obj2, &floor, &rWall, &lWall, &roof, new Object(c1) };
	Vec2 c = Vec2(HEIGHT / 2, WIDTH / 2);
	clock_t t;
};