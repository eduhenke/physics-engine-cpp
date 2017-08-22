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
	Poly p1 = std::vector<Vec2>{ Vec2(110,120), Vec2(160,120), Vec2(170,180), Vec2(130,200), Vec2(100, 140) };
	Poly p2 = std::vector<Vec2>{ Vec2(100,0), Vec2(200,100), Vec2(0,100) };
	Circle c1 = Circle(Vec2(-200, -150), 30);
	Object obj1 = Object(p1), obj2 = Object(p2);
	Vec2 c = Vec2(HEIGHT / 2, WIDTH / 2);
};