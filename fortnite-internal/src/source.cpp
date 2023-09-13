#include"../ext/ext.h"
#include<iostream>

NTSTATUS WINAPI FeaturesThread(LPVOID Buffer)
{
	static bool init_features = false;
	while (!init_features)
	{
		DWORD64 ModuleBase = (DWORD64)pGetModule->FindModuleHandle("FortniteClient-Win64-Shipping.exe");
		DWORD64 LocalEntity = *(DWORD64*)(ModuleBase + pOffsets.local_player);
		
		for (int index = 0; index < 100; index++)
		{
			DWORD64 Entity = *(DWORD64*)(ModuleBase + pOffsets.cl_entitylist + (index * 32));
			if (Entity == NULL || Entity == LocalEntity)
				return FALSE;

			if (!features::aimbot::enable_aimbot)
				return FALSE;

			if (features::aimbot::enable_memory_aimbot)
			{
				
			}
		}

	}
	return TRUE;
}

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
	pThread->NtCreateThread((LPTHREAD_START_ROUTINE)FeaturesThread, 0, 0);

	return TRUE;
}