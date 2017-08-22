/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include <algorithm>
#include "Mat2.h"


using namespace std;

MainWindow* wnd;
Graphics* gfx;
Game::Game( MainWindow& w )
{
	wnd = &w; gfx = new Graphics(w);
	//AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	obj1.veloc = Vec2(1, -1)/3.8;
	obj2.veloc = Vec2(0, 1).GetNormalized()/2;
	obj1.mass = 0.1f;
	//obj2.veloc = Vec2(0, -1)/2;
	c1.move(Vec2(100, 200));
	p2.move(Vec2(10, -200));
}

void Game::Go()
{
	gfx->BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx->EndFrame();
}

void Game::UpdateModel()
{
	static Vec2 center(WIDTH / 2, HEIGHT / 2);
	//obj2.shape.rotate(0.002);
	obj1.shape.draw();
	obj2.shape.draw();
	obj1.handleCollision(obj2);
	Vec2 mousePos = gfx->ConvertToScene(Vec2(wnd->mouse.GetPos()));
	if (wnd->mouse.LeftIsPressed())
	{
		obj1.shape.goTo(mousePos);
	}
	if (wnd->mouse.RightIsPressed())
	{
		obj2.shape.goTo(mousePos);
	}
}

void Game::ComposeFrame()
{
}

//Poly diff = Geometry::minkowskiDiff(p1, p2);
//diff.move(center);
//gfx->DrawPolygon(diff, Colors::Green);
//Vec2 d, f;
//Geometry::closestPoints(p2, diff, d, f);
//gfx->DrawVector(d, f, Colors::Red);