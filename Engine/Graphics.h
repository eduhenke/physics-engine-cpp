/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Graphics.h																			  *
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
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Vec2.h"
#include <assert.h>
#include <vector>

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x, y, z;		// position
		float u, v;			// texcoords
	};
public:
	Graphics(class HWNDKey& key);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void BeginFrame();
	Vec2 ConvertToDraw(const Vec2& v)
	{
		return Vec2(ScreenWidth / 2 + v.x, ScreenHeight / 2 - v.y);
	}
	Vec2 ConvertToScene(const Vec2& v)
	{
		return Vec2(-(float)ScreenWidth / 2 + v.x, ScreenHeight / 2 - v.y);
	}
	void DrawVector(const Vec2& v, Color col);
	void DrawVector(Vec2 v, Vec2 c, Color col);
	void DrawPolygon(const std::vector<Vec2>& vertices, Color c)
	{
		for (int i = 1; i < vertices.size()-1; i++)
		{
			DrawTriangle(vertices[0], vertices[i], vertices[i+1], c);
		}
	};
	void DrawCircle(const Vec2& pos, float r, Color c)
	{
		DrawCircle(pos.x, pos.y, r, c);
	}
	void DrawCircle(int x, int y, float r, Color c);
	void DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);
	void DrawLine(const Vec2& p1, const Vec2& p2, Color c)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, c);
	}
	void DrawLine(float x1, float y1, float x2, float y2, Color c);
	void PutPixel(const Vec2& p, Color c)
	{
		PutPixel((int)p.x, (int)p.y, c);
	}
	void PutPixel(int x, int y, int r, int g, int b)
	{
		PutPixel(x, y, { unsigned char(r),unsigned char(g),unsigned char(b) });
	}
	void PutPixel(int x, int y, Color c);
	~Graphics();
private:
	void DrawFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);
	void DrawFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr unsigned int ScreenWidth = 960u;
	static constexpr unsigned int ScreenHeight = 480u;
};