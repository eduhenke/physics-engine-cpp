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
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
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
}

void Game::ComposeFrame()
{
	gfx->DrawPolygon(p1, Colors::Cyan);
	gfx->DrawPolygon(p2, Colors::Magenta);
	static Vec2 center(WIDTH / 2, HEIGHT / 2);
	Vec2 mousePos = Vec2(wnd->mouse.GetPos()).Clamp(0, 0, WIDTH, HEIGHT);

	p1.rotate(0.02);
	Poly diff = Geometry::minkowskiDiff(p1, p2);
	diff.move(diff.center()+center);
	gfx->DrawPolygon(diff, Colors::Green);
	Vec2 d, f;
	Geometry::closestPoints(p2, diff, d, f);
	gfx->DrawVector(d, f, Colors::Red);
	mousePos.x = max(0, min((int)WIDTH-1, wnd->mouse.GetPosX()));
	mousePos.y = max(0, min((int)HEIGHT-1, wnd->mouse.GetPosY()));
	p2.move(mousePos);
}
