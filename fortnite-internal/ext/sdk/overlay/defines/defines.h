#pragma once
#include<d3d11.h>
#include<dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

typedef HRESULT(__stdcall* oPresent_t)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(__stdcall* oWndProc_t)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL(__stdcall* oCursor_t)(bool);

class D3D11
{
public:
	oPresent_t oPresent = 0;
	oWndProc_t oWndProc = 0;
	oCursor_t oCursor = 0;
public:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
public:
	HWND hWindow = 0;
	WNDCLASSEX WindowClass;
	HWND WindowHwnd = 0;
}; D3D11* pD3D11 = new D3D11();