#include "pch.h"
#include "Registry.h"
#include "Memory.h"

/**
 * Queries a registry value using DMA
 * 
 * Reads a registry value from the target process using VMMDLL:
 * 1. Allocates buffer for the value
 * 2. Queries the value using VMMDLL_WinReg_QueryValueExU
 * 3. Converts the result to a string
 * 
 * @param path - Registry path to query (e.g. "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion")
 * @param type - Expected value type
 * @return Retrieved value as string, or empty string on failure
 */
std::string c_registry::QueryValue(const char* path, e_registry_type type)
{
	// Allocate buffer for registry value
	BYTE buffer[0x128];
	DWORD _type = (DWORD)type;
	DWORD size = sizeof(buffer);

	// Query registry value using DMA
	bool result = VMMDLL_WinReg_QueryValueExU(TargetProcess.vHandle, CC_TO_LPSTR(path), &_type, buffer, &size);
	if (!result)
	{
		LOG("[!] failed QueryValueExU call\n");
		return "";
	}

	// Convert wide string result to regular string
	std::wstring wstr = std::wstring((wchar_t*)buffer);
	return std::string(wstr.begin(), wstr.end());
}
