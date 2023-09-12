#pragma once
#include"DX11/DX11.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_dx11.h"
#include"imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(pD3D11->hWindow);
		ImGui_ImplDX11_Init(pD3D11->pDevice, pD3D11->pContext);
		return TRUE;
	}
	NTSTATUS WINAPI RenderOverlay(LPCSTR WndTitle)
	{
		ImGui::Begin(WndTitle, nullptr, 0);
		ImGui::SetWindowSize(ImVec2(450, 450));
		ImGui::End();
		return TRUE;
	}
}; Overlay* pOverlay = new Overlay();

namespace CursorHooks
{
	NTSTATUS WINAPI InitHook(LPVOID pFunction, LPVOID pDetour, LPVOID* pOriginal)
	{
		Hook* pHook = new Hook();
		if (pHook->SpoofCreateHook(pFunction, pDetour, pOriginal) != MH_OK)
			return FALSE;
		if (pHook->SpoofEnableHook(pFunction) != MH_OK)
			return FALSE;
		return TRUE;
	}

	NTSTATUS WINAPI RemoveHook(LPVOID pFuction)
	{
		Hook* pHook = new Hook();
		pHook->SpoofDiableHook(pFuction);
		pHook->SpoofRemoveHook(pFuction);
	}
}

namespace Detours
{
	BOOL WINAPI CursorDetour(bool show) {
		if (pOverlay->render_overlay)
			return FALSE;
		return pD3D11->oCursor(show);
	}

	LRESULT WINAPI WndProcDetour(HWND hWindow, UINT uInt, WPARAM wParam, LPARAM lParam)
	{
		if (pOverlay->render_overlay) {
			ImGui_ImplWin32_WndProcHandler(hWindow, uInt,  wParam,  lParam);
			return TRUE;
		}
		return pDX11CreateWindow->CallOldWndProc(pD3D11->oWndProc, hWindow, uInt, wParam, lParam);
	}

	HRESULT WINAPI PresentDetour(IDXGISwapChain* pSwapChain, UINT Sync, UINT Flags)
	{
		if (!pOverlay->init_imgui)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&pD3D11->pDevice)))
			{
				ImGui::CreateContext();
				pD3D11->pDevice->GetImmediateContext(&pD3D11->pContext);
				DXGI_SWAP_CHAIN_DESC desc;
				pSwapChain->GetDesc(&desc);
				pD3D11->hWindow = desc.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pD3D11->pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pD3D11->pRenderTargetView);
				pBackBuffer->Release();
				ImGui::GetIO().ImeWindowHandle = pD3D11->hWindow;
				pD3D11->oWndProc = (oWndProc_t)SetWindowLongPtr(pD3D11->hWindow, GWLP_WNDPROC, (LONG_PTR)WndProcDetour);
				pOverlay->InitImGui();
				pOverlay->init_imgui = true;
			}
			else
				return pD3D11->oPresent(pSwapChain, Sync, Flags);
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		if (m_pMemory->NtGetAsyncKeyState(VK_INSERT) & 0x1) { pOverlay->render_overlay = !pOverlay->render_overlay; }
		if (pOverlay->render_overlay) { pOverlay->RenderOverlay("Fortnite Internal Base"); }

		ImGui::EndFrame();
		ImGui::Render();
		pD3D11->pContext->OMSetRenderTargets(1, &pD3D11->pRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return pD3D11->oPresent(pSwapChain, Sync, Flags);
	}
}
