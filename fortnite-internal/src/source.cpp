#include"../ext/ext.h"
#include<iostream>

NTSTATUS WINAPI MainThread(LPVOID buffer)
{
	bool init_hook = false;
	while (!init_hook)
	{
		DX11* pDX11 = new DX11();
		if (pDX11->Initialize() == TRUE)
		{
			Hook* pHook = new Hook();
			pHook->CreateHook(8, (LPVOID*)&pD3D11->oPresent, Detours::PresentDetour);
			init_hook = true;
		}
	}
	return TRUE;
}

NTSTATUS WINAPI DllMain(HINSTANCE hInstance, DWORD64 hReasons, LPVOID hBuffer)
{
	if (hReasons != DLL_PROCESS_ATTACH)
		return FALSE;
	
	Thread* pThread = nullptr;
	pThread->NtCreateThread((LPTHREAD_START_ROUTINE)MainThread, 0, 0);

	return TRUE;
}