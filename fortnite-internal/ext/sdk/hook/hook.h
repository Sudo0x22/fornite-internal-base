#pragma once
#include<cassert>
#include<Windows.h>
#include<cstdint>
#include"minhook/include/MinHook.h"
#include"../utils/spoofer/spoofer.hpp"

typedef uint64_t uint150_t;
static uint150_t* Table = nullptr;

class SpoofHook
{
public:
	MH_STATUS SpoofCreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* pOriginal){ return SpoofReturn(MH_CreateHook, pTarget, pDetour, pOriginal); }
	MH_STATUS SpoofEnableHook(LPVOID pTarget) { return SpoofReturn(MH_EnableHook, pTarget); }
	MH_STATUS SpoofDiableHook(LPVOID pTarget) { return SpoofReturn(MH_DisableHook, pTarget); }
	MH_STATUS SpoofRemoveHook(LPVOID pTarget) { return SpoofReturn(MH_RemoveHook, pTarget); }
};

class Hook : public SpoofHook
{
public:
	BOOL CreateHook(uint64_t nIndex,  LPVOID* pOriginal, LPVOID pDetour)
	{
		assert(nIndex > 0 && pDetour != NULL && pOriginal != NULL);
		LPVOID pFunction = (LPVOID)Table[nIndex];
		if (this->SpoofCreateHook(pFunction, pDetour, pOriginal) != MH_STATUS::MH_OK || this->SpoofEnableHook(pFunction) != MH_STATUS::MH_OK)
			return FALSE;
		return TRUE;
	}

	BOOL CreateHookEx(LPVOID pFunction, LPVOID pDetour, LPVOID* pOriginal)
	{
		if (this->SpoofCreateHook(pFunction, pDetour, pOriginal) == MH_STATUS::MH_OK && this->SpoofEnableHook(pDetour) == MH_STATUS::MH_OK)
			return FALSE;
		return TRUE;
	}
};