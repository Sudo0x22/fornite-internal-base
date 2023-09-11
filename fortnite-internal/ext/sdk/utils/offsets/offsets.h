#pragma once
#include<cstdint>
#include<Windows.h>
#include<thread>
#include<chrono>

class Offsets
{
public:
	uintptr_t local_player = 0x0;
	uintptr_t cl_entitylist = 0x0;
}; Offsets pOffsets;

class PlayerOffsets : public Offsets
{
public:
	uintptr_t dw_iHealth = 0x0;
	uintptr_t dw_iShield = 0x0;
	uintptr_t dw_sName = 0x0;
	uintptr_t dw_iAmmoCount = 0x0;
}; PlayerOffsets pPlayerOffsets;

class WorldOffsets : public Offsets
{
public:
	uintptr_t world_offset = 0x0;
	uintptr_t ammo_offset = 0x0;
}; WorldOffsets pWorldOffsets;