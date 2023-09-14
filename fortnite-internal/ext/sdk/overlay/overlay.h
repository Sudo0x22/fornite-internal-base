#pragma once
#include"DX11/DX11.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_dx11.h"
#include"imgui/imgui_impl_win32.h"
#include"../vars/vars.h"
#include"icon/icon.h"
#include"../utils/module/lazy_importer.h"
#include"../utils/module/xorstr.h"
#include"../utils/module/SkCrypt.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Overlay
{
public:
	DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	enum class MenuTabs
	{
		Aimbot,
		Visuals,
		Binds,
		Misc,
		Configs,
	};

	MenuTabs CurrentTab = MenuTabs::Aimbot;

public:
	BOOL init_imgui = FALSE;
	BOOL render_overlay = FALSE;
	BOOL render_cursor = FALSE;
	BOOL capture_cursor = FALSE;
	bool close_buttton = true;
public:
	NTSTATUS WINAPI InitImGui()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(pD3D11->hWindow);
		ImGui_ImplDX11_Init(pD3D11->pDevice, pD3D11->pContext);
		return TRUE;
	}

	NTSTATUS WINAPI InitStyle()
	{
		ImGuiStyle* pStyle = &ImGui::GetStyle();
		pStyle->WindowTitleAlign = ImVec2(0.5, 0.5);
		pStyle->WindowRounding = 4.0f;

		pStyle->Colors[ImGuiCol_Border] = ImColor(15, 150, 150, 255);
		pStyle->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 250);
		
		pStyle->Colors[ImGuiCol_TitleBg] = ImColor(0, 0, 0, 250);
		pStyle->Colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0, 250);

		pStyle->Colors[ImGuiCol_SliderGrab] = ImColor(15, 150, 150, 255);
		pStyle->Colors[ImGuiCol_SliderGrabActive] = ImColor(15, 150, 150, 255);

		pStyle->Colors[ImGuiCol_Separator] = ImColor(15, 150, 150, 255);
		pStyle->Colors[ImGuiCol_SeparatorActive] = ImColor(15, 150, 150, 255);
		pStyle->Colors[ImGuiCol_SeparatorHovered] = ImColor(15, 150, 150, 255);

		pStyle->Colors[ImGuiCol_FrameBg] = ImColor(28, 28, 28, 255);
		pStyle->Colors[ImGuiCol_FrameBgHovered] = ImColor(26, 26, 26, 255);
		pStyle->Colors[ImGuiCol_FrameBgActive] = ImColor(23, 23, 23, 255);

		return TRUE;

		// FortniteClient-Win64-Shipping_EAC_EOS.exe
		// fortnite-internal.dll
	}

	NTSTATUS WINAPI AimbotTab() { 
		ImGui::Checkbox(skCrypt("Toggle Slient Aim").decrypt(), &features::aimbot::enable_slient_aim);
		
		ImGui::Checkbox(skCrypt("Toggle Lock Aim").decrypt(), &features::aimbot::enable_memory_aimbot);
		ImGui::Checkbox(skCrypt("Toggle Aim Fov").decrypt(), &features::aimbot::enable_fov_circle);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SliderInt(skCrypt("Aim Fov").decrypt(), &features::aimbot::aim_fov, 0, 180);

		return TRUE; 
	}
	NTSTATUS WINAPI VisualsTab() { 
		ImGui::Checkbox(skCrypt("Toggle Outline").decrypt(), &features::esp::enable_outline);
		ImGui::Checkbox(skCrypt("Toggle Box Esp").decrypt(), &features::esp::enable_box);
		ImGui::Checkbox(skCrypt("Toggle Health Esp").decrypt(), &features::esp::enable_health);
		return TRUE; 
	}
	NTSTATUS WINAPI MiscTab() { return TRUE; }

	NTSTATUS WINAPI RenderOverlay(LPCSTR WndTitle)
	{
		auto backgound = ImGui::GetBackgroundDrawList();
		backgound->AddRectFilled(ImVec2{ 0,0 }, ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImColor(15, 15, 15, 150));

		if (ImGui::Begin(WndTitle, &this->close_buttton, this->window_flags))
		{
			ImGui::SetWindowSize(ImVec2(450, 450));

			INT height = 40, width = 100;
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, width + 30);

			static MenuTabs Tab = MenuTabs::Aimbot;

			{
				static ImVec4 InActive = ImVec4(0.09f, 0.09f, 0.09f, 0.88f);
				static ImVec4 Active = ImVec4(0.0f, 0.5f, 0.5f, 0.9f);

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Aimbot ? Active : InActive);
				if (ImGui::Button(skCrypt("Aimbot").decrypt(), ImVec2(width, height)))
					Tab = MenuTabs::Aimbot;

				ImGui::PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Visuals ? Active : InActive);
				if (ImGui::Button(skCrypt("Visuals").decrypt(), ImVec2(width, height)))
					Tab = MenuTabs::Visuals;

				ImGui::PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Misc ? Active : InActive);
				if (ImGui::Button(skCrypt("Misc").decrypt(), ImVec2(width, height)))
					Tab = MenuTabs::Misc;

				ImGui::PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Binds ? Active : InActive);
				if (ImGui::Button(skCrypt("Binds").decrypt(), ImVec2(width, height)))
					Tab = MenuTabs::Binds;

				ImGui::PushStyleColor(ImGuiCol_Button, Tab == MenuTabs::Configs ? Active : InActive);
				if (ImGui::Button(skCrypt("Configs").decrypt(), ImVec2(width, height)))
					Tab = MenuTabs::Configs;


				ImGui::PopStyleColor(5);
			}

			ImGui::NextColumn();

			{
				switch (Tab)
				{
				case MenuTabs::Aimbot:
					AimbotTab();
					break;
				case MenuTabs::Visuals:
					VisualsTab();
					break;
				case MenuTabs::Misc:
					MiscTab();
					break;
				case MenuTabs::Binds:
					break;
				case MenuTabs::Configs:
					break;
				}
			}

			ImGui::End();
		}

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
		return TRUE;
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
				pOverlay->InitStyle();
				pOverlay->init_imgui = true;
			}
			else
				return pD3D11->oPresent(pSwapChain, Sync, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::GetIO().MouseDrawCursor = pOverlay->render_overlay;
		ImGui::GetIO().WantCaptureMouse = pOverlay->render_overlay;

		pOverlay->render_cursor = ImGui::GetIO().WantCaptureMouse;
		pOverlay->capture_cursor = ImGui::GetIO().MouseDrawCursor;

		if (m_pMemory->NtGetAsyncKeyState(VK_INSERT) & 0x1) { pOverlay->render_overlay = !pOverlay->render_overlay; }
		if (pOverlay->render_overlay) { pOverlay->RenderOverlay(skCrypt("settings").decrypt()); }

		if (features::aimbot::enable_fov_circle)
		{
			auto vList = ImGui::GetBackgroundDrawList();
			vList->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), features::aimbot::aim_fov * 5, ImColor(15,150,150,255), 100, 1.f);
		}

		if (pOverlay->render_cursor || pOverlay->capture_cursor) { CursorHooks::InitHook(&SetCursorPos, CursorDetour, reinterpret_cast<LPVOID*>(&pD3D11->oCursor)); }
		else { CursorHooks::RemoveHook(&SetCursorPos); }

		ImGui::EndFrame();
		ImGui::Render();
		pD3D11->pContext->OMSetRenderTargets(1, &pD3D11->pRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return pD3D11->oPresent(pSwapChain, Sync, Flags);
	}
}
