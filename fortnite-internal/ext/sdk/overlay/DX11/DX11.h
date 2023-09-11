#pragma once
#include"../../hook/hook.h"
#include"../../utils/utils.h"
#include"../defines/defines.h"

class DX11
{
public:
	NTSTATUS WINAPI InitWindow();
	NTSTATUS WINAPI DeleteWindow();
	NTSTATUS WINAPI Initialize();
};

class DX11CreateWindow : public DX11
{
public:
	ATOM WINAPI DX11RegisterWnd(const WNDCLASSEX* oWndClass) {
		return SpoofReturn(RegisterClassEx, oWndClass);
	}
	ATOM WINAPI DX11UnregisterWnd(LPCWSTR Class, HINSTANCE hInstance) {
		return SpoofReturn(UnregisterClass, Class, hInstance);
	}
	BOOL WINAPI DX11DeleteWindow(HWND hWindow) {
		return SpoofReturn(DestroyWindow, hWindow);
	}
	HWND WINAPI DX11CreateWindowEx(DWORD Exit, LPCWSTR Class, LPCWSTR Name, DWORD Style,
		int x, int y, int w, int h, HWND hWindow, HMENU hMenu, HINSTANCE hInstance, LPVOID Param) {
		return SpoofReturn(CreateWindowExW, Exit, Class, Name, Style, x, y, w, h, hWindow, hMenu, hInstance, Param);
	}
	LRESULT WINAPI CallOldWndProc(WNDPROC oWndProc, HWND hWindow, UINT uInt, WPARAM wParam, LPARAM lParam) {
		return SpoofReturn(CallWindowProcW, oWndProc, hWindow, uInt, wParam, lParam);
	}

}; DX11CreateWindow* pDX11CreateWindow = new DX11CreateWindow();

NTSTATUS WINAPI DX11::InitWindow()
{
	pD3D11->WindowClass.cbSize = sizeof(WNDCLASSEX);
	pD3D11->WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	pD3D11->WindowClass.lpfnWndProc = DefWindowProc;

	pD3D11->WindowClass.cbClsExtra = 0;
	pD3D11->WindowClass.cbWndExtra = 0;
	pD3D11->WindowClass.hInstance = pGetModule->FindModuleHandle(NULL);

	pD3D11->WindowClass.hIcon = NULL;
	pD3D11->WindowClass.hCursor = NULL;
	pD3D11->WindowClass.hbrBackground = NULL;

	pD3D11->WindowClass.lpszMenuName = NULL;
	pD3D11->WindowClass.lpszClassName = L"OK";
	pD3D11->WindowClass.hIconSm = NULL;

	pDX11CreateWindow->DX11RegisterWnd(&pD3D11->WindowClass);	
	pD3D11->WindowHwnd = pDX11CreateWindow->DX11CreateWindowEx(NULL, pD3D11->WindowClass.lpszClassName, L"Fornite Window", WS_OVERLAPPEDWINDOW,
		0, 0, 100, 100, NULL, NULL, pD3D11->WindowClass.hInstance, NULL);
	if (pD3D11->WindowHwnd == NULL) { return FALSE; }
	return TRUE;
}

NTSTATUS WINAPI DX11::DeleteWindow()
{
	pDX11CreateWindow->DX11DeleteWindow(pD3D11->WindowHwnd);
	pDX11CreateWindow->DX11UnregisterWnd(pD3D11->WindowClass.lpszClassName, pD3D11->WindowClass.hInstance);
	if (pD3D11->WindowHwnd != NULL) { return FALSE; }
	return TRUE;
}

NTSTATUS WINAPI DX11::Initialize()
{
	if (this->InitWindow() == FALSE) { this->DeleteWindow(); return FALSE; }

	HINSTANCE hModule = pGetModule->FindModuleHandle("d3d11.dll");
	LPVOID D3D11CreateDeviceSwapChain = pGetModule->FindModuleProcAddr(hModule, "D3D11CreateDeviceAndSwapChain");

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL refreshRate;
	refreshRate.Numerator = 60;
	refreshRate.Denominator = 1;

	DXGI_MODE_DESC bufferDesc;
	bufferDesc.Width = 100;
	bufferDesc.Height = 100;
	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = pD3D11->WindowHwnd;
	swapChainDesc.Windowed = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	if (((long(__stdcall*)(
		IDXGIAdapter*,
		D3D_DRIVER_TYPE,
		HMODULE,
		UINT,
		const D3D_FEATURE_LEVEL*,
		UINT,
		UINT,
		const DXGI_SWAP_CHAIN_DESC*,
		IDXGISwapChain**,
		ID3D11Device**,
		D3D_FEATURE_LEVEL*,
		ID3D11DeviceContext**))(D3D11CreateDeviceSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0)
	{
		this->DeleteWindow();
		return false;
	}

	Table = (uint150_t*)::calloc(205, sizeof(uint150_t));
	m_pMemory->SafeCopy(Table, *(uint150_t**)swapChain, 18 * sizeof(uint150_t));
	m_pMemory->SafeCopy(Table + 18, *(uint150_t**)device, 43 * sizeof(uint150_t));
	m_pMemory->SafeCopy(Table + 18 + 43, *(uint150_t**)context, 144 * sizeof(uint150_t));

	MH_Initialize();

	swapChain->Release();
	swapChain = NULL;

	device->Release();
	device = NULL;

	context->Release();
	context = NULL;

	this->DeleteWindow();
	return TRUE;
}