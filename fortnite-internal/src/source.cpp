#include"../ext/ext.h"
#include<iostream>

NTSTATUS WINAPI MainThread(LPVOID buffer)
{
	AllocConsole();
	FILE* pStreamIn = { 0 };
	freopen_s(&pStreamIn, "CONOUT", "w", stdout);

	DWORD64 ModuleBase = (DWORD64)pGetModule->FindModuleHandle("FortniteClient-Win64-Shipping.exe");
	std::cout << "Module Base: 0x" << std::hex << ModuleBase << "\n";

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