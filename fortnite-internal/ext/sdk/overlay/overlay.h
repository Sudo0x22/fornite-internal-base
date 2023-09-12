#pragma once
#include"DX11/DX11.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_dx11.h"
#include"imgui/imgui_impl_win32.h"

class Overlay
{
public:
	BOOL init_imgui = FALSE;
	BOOL render_overlay = FALSE;
	BOOL render_cursor = FALSE;
	BOOL capture_cursor = FALSE;
public:
	NTSTATUS WINAPI InitImGui()
	{
		return TRUE;
	}
	NTSTATUS WINAPI RenderOverlay(LPCSTR WndTitle)
	{
		return TRUE;
	}
};

namespace Detours
{
	BOOL WINAPI CursorDetour(bool show) {
		return pD3D11->oCursor(show);
	}

	LRESULT WINAPI WndProcDetour(HWND hWindow, UINT uInt, WPARAM wParam, LPARAM lParam)
	{
		return pDX11CreateWindow->CallOldWndProc(pD3D11->oWndProc, hWindow, uInt, wParam, lParam);
	}

	HRESULT WINAPI PresentDetour(IDXGISwapChain* pSwapChain, UINT Sync, UINT Flags)
	{
		if (m_pMemory->NtGetAsyncKeyState(VK_INSERT) & 0x1) { AllocConsole(); }
		return pD3D11->oPresent(pSwapChain, Sync, Flags);
	}
}
