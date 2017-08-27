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
	obj1.veloc = Vec2(200, -100);
	obj1.shape.move(Vec2(-300, -200));
	obj2.shape.move(Vec2(0, -200));
	obj2.mass = 5;
	obj2.momentOfInertia *= obj2.mass;
	t = clock();
}

void Game::Go()
{
	gfx->BeginFrame();
	ComposeFrame(); // 'compose frame' before 'update model', because debugging is easier
	t = clock() - t;
	UpdateModel();
	t = clock();
	gfx->EndFrame();
}

void Game::UpdateModel()
{
	static float dt;
	dt = (float)t / CLOCKS_PER_SEC;
	for (Object* obj : scene)
	{
		obj->accel = GRAVITY;
		obj->angAccel = 0.0f;
		Vec2 C = obj->shape.center();
		gfx->DrawVector(obj->veloc + C, C, Colors::Red);
	}
	for (int i = 0; i < scene.size() - 1; i++)
	{
		for (int j = i + 1; j < scene.size(); j++)
		{
			scene[i]->handleCollision(*scene[j], dt);
		}
		scene[i]->move(dt);
	}
	scene.back()->move(dt);
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
	for (Object* obj : scene)
	{
		obj->shape.draw();
	}
}

//Poly diff = Geometry::minkowskiDiff(p1, p2);
//diff.move(center);
//gfx->DrawPolygon(diff, Colors::Green);
//Vec2 d, f;
//Geometry::closestPoints(p2, diff, d, f);
//gfx->DrawVector(d, f, Colors::Red);