#pragma once
#include<Windows.h>
#include<stdio.h>
#include"../spoofer/spoofer.hpp"

typedef struct SystemMemory
{
	DWORD64 dwEP;
	LPVOID dwParam;
	BOOL dwRead;
	BOOL dwWrite;
	SIZE_T dwSize;
	SIZE_T* dwBytes;
	HANDLE dwProcess;
} SystemMemory, * PSystemMemory;
typedef DWORD(*InitMem)(LPVOID Param);

NTSTATUS WINAPI InitThread(PSystemMemory pMemory)
{
	if (pMemory != NULL && pMemory->dwEP != NULL)
	{
		InitMem CallMem = (InitMem)pMemory->dwEP;
		CallMem(pMemory->dwParam);
	}
	return TRUE;
}

class Module
{
public:
	DWORD64 WINAPI GetModuleSize(DWORD64 ModuleImage)
	{
		IMAGE_DOS_HEADER dos_header = *(IMAGE_DOS_HEADER*)ModuleImage;
		IMAGE_NT_HEADERS nt_headers = *(IMAGE_NT_HEADERS*)(ModuleImage + dos_header.e_lfanew);
		return (DWORD64)nt_headers.OptionalHeader.SizeOfImage;
	}
	PBYTE WINAPI GetModuleBytes(LPVOID BaseAddr, DWORD64 BaseSize, DWORD64 Hex)
	{
		PBYTE ModuleBytes = (PBYTE)BaseAddr + BaseSize - Hex;
		return ModuleBytes;
	}
};

class GetModule : public Module
{
public:
	HINSTANCE WINAPI FindModuleHandle(LPCSTR ModuleImage)
	{
		HINSTANCE hModule = SpoofReturn(GetModuleHandleA, ModuleImage);
		return hModule;
	}
	FARPROC WINAPI FindModuleProcAddr(HINSTANCE hModule, LPCSTR ModuleProc)
	{
		FARPROC hProcAddr = SpoofReturn(GetProcAddress, hModule, ModuleProc);
		return hProcAddr;
	}
}; GetModule* pGetModule = new GetModule();

class Memory : public GetModule
{
public:
	LPVOID SafeCopy(LPVOID Src, const void* Det, size_t Size)
	{
		return SpoofReturn(memcpy, Src, Det, Size);
	}
	BOOL WINAPI NtVirtualProctect(LPVOID Src, SIZE_T Size, DWORD Protect, PDWORD dwProtect)
	{
		return SpoofReturn(VirtualProtect, Src, Size, Protect, dwProtect);
	}
	LPVOID WINAPI NtVirtualAlloc(LPVOID Src, SIZE_T Size, DWORD AllocType, DWORD Protect)
	{
		return SpoofReturn(VirtualAlloc, Src, Size, AllocType, Protect);
	}
	BOOL WINAPI NtGetAsyncKeyState(INT KeyCode) {
		return SpoofReturn(GetAsyncKeyState, KeyCode);
	}
}; Memory* m_pMemory = new Memory();

class Thread : public Memory
{
public:
	NTSTATUS WINAPI NtCreateThread(LPTHREAD_START_ROUTINE EntryPoint, LPVOID Buffer, LPDWORD ThreadId)
	{
		HINSTANCE hInstance = this->FindModuleHandle("ntdll.dll");
		DWORD64 hSize = this->GetModuleSize((DWORD64)hInstance);
		PBYTE hBytes = this->GetModuleBytes((LPVOID)hInstance, hSize, 0x400);

		DWORD dwProtect;
		this->NtVirtualProctect(hBytes, 0x100, PAGE_EXECUTE_READWRITE, &dwProtect);
		
		PSystemMemory pMemory = (PSystemMemory)this->NtVirtualAlloc(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		pMemory->dwEP = (DWORD64)(EntryPoint);
		pMemory->dwParam = Buffer;

		this->SafeCopy((LPVOID)hBytes, (LPVOID)InitThread, 0x100);
		CreateRemoteThread(GetCurrentProcess(), 0, 0x100, (LPTHREAD_START_ROUTINE)hBytes, pMemory, 0, ThreadId);
		return TRUE;
	}
};