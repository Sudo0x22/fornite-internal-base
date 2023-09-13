#pragma once
#include<cstdint>

namespace features
{
	namespace esp
	{
		bool enable_esp = false;
		bool enable_outline = false;
		bool enable_box = false;
		bool enable_name = false;
		bool enable_health = false;
	}

	namespace aimbot
	{
		bool enable_aimbot = false;
		bool enable_memory_aimbot = false;
		bool enable_slient_aim = false;
		bool enable_fov_circle = false;
		int aim_fov = 5;
	}
}