#include "pch.h"
#include "InputManager.h"
#include "Registry.h"
#include "Memory/Memory.h"

namespace Keyboard
{
	uint64_t gafAsyncKeyStateExport = 0;  // Address of gafAsyncKeyState in win32kbase.sys
	uint8_t state_bitmap[64]{ };          // Current keyboard state bitmap
	uint8_t previous_state_bitmap[256 / 8]{ };  // Previous keyboard state bitmap
	uint64_t win32kbase = 0;              // Base address of win32kbase.sys
	int win_logon_pid = 0;                // Process ID of winlogon.exe
}

/**
 * Initializes keyboard state tracking
 * 
 * Sets up DMA access to keyboard state by:
 * 1. Getting Windows version from registry
 * 2. Finding winlogon.exe process
 * 3. Locating gafAsyncKeyState in win32kbase.sys
 * 4. Setting up state tracking variables
 * 
 * @return true if initialization successful
 */
bool Keyboard::InitKeyboard()
{
	// Get Windows version from registry
	c_registry registry;
	std::string win = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	if (win == "")
		return false;
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;

	// Get winlogon process ID
	win_logon_pid = TargetProcess.GetPidFromName("winlogon.exe");

	// Windows 11 (build > 22000) handling
	if (Winver > 22000)
	{
		// Find gafAsyncKeyState in win32ksgd.sys
		auto pids = TargetProcess.GetPidListFromName("csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];
			uintptr_t tmp = VMMDLL_ProcessGetModuleBaseU(TargetProcess.vHandle, pid, (LPSTR)"win32ksgd.sys");
			uintptr_t g_session_global_slots = tmp + 0x3110;
			uintptr_t user_session_state = TargetProcess.Read<uintptr_t>(TargetProcess.Read<uintptr_t>(TargetProcess.Read<uintptr_t>(g_session_global_slots, pid), pid), pid);
			gafAsyncKeyStateExport = user_session_state + 0x3690;
			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
				break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
	// Windows 10 handling
	else
	{
		printf("win10\n");
		// Find gafAsyncKeyState in win32kbase.sys
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(TargetProcess.vHandle, TargetProcess.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, (LPSTR)"win32kbase.sys", &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}
		printf("eat_map->cMap: %d\n", eat_map->cMap);
		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;
				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
}

/**
 * Updates the current keyboard state
 * 
 * Reads keyboard state from kernel memory and updates:
 * 1. Previous state bitmap
 * 2. Current state bitmap
 * 3. Key press tracking
 */
void Keyboard::UpdateKeys()
{
	// Save previous state
	uint8_t previous_key_state_bitmap[64] = { 0 };
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	// Read current state from kernel
	VMMDLL_MemReadEx(TargetProcess.vHandle, win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, (PBYTE)&state_bitmap, 64, NULL, VMMDLL_FLAG_NOCACHE);

	// Update key press tracking
	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

// Timer for key state updates
auto start = std::chrono::system_clock::now();

/**
 * Checks if a key is currently pressed
 * 
 * Updates keyboard state if needed and checks:
 * 1. If keyboard tracking is initialized
 * 2. If update interval has elapsed
 * 3. Current state of requested key
 * 
 * @param virtual_key_code - Windows virtual key code to check
 * @return true if key is pressed
 */
bool Keyboard::IsKeyDown(uint32_t virtual_key_code)
{
	// Check initialization
	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;

	// Update state if needed
	if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(1))
	{
		UpdateKeys();
		start = std::chrono::system_clock::now();
	}

	// Check key state
	return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}
