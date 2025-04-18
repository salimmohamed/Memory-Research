#pragma once
#include "pch.h"

/**
 * Registry value types supported by Windows
 * Maps to Windows REG_* constants
 */
enum class e_registry_type
{
	none = REG_NONE,                    // No value type
	sz = REG_SZ,                        // Null-terminated string
	expand_sz = REG_EXPAND_SZ,          // Null-terminated string with environment variables
	binary = REG_BINARY,                // Binary data
	dword = REG_DWORD,                  // 32-bit number
	dword_little_endian = REG_DWORD_LITTLE_ENDIAN,  // 32-bit number in little-endian format
	dword_big_endian = REG_DWORD_BIG_ENDIAN,        // 32-bit number in big-endian format
	link = REG_LINK,                    // Symbolic link
	multi_sz = REG_MULTI_SZ,            // Array of null-terminated strings
	resource_list = REG_RESOURCE_LIST,  // Device driver resource list
	full_resource_descriptor = REG_FULL_RESOURCE_DESCRIPTOR,  // Hardware setting
	resource_requirements_list = REG_RESOURCE_REQUIREMENTS_LIST,  // Hardware resource list
	qword = REG_QWORD,                  // 64-bit number
	qword_little_endian = REG_QWORD_LITTLE_ENDIAN   // 64-bit number in little-endian format
};

/**
 * Converts a wide string (LPWSTR) to a C-style string
 * @param in - Input wide string
 * @return Converted C-style string
 */
inline const char* LPWSTR_TO_CC(LPWSTR in)
{
	char buffer[500];
	wcstombs(buffer, in, 500);

	return buffer;
}

/**
 * Converts a C-style string to a Windows LPSTR
 * @param in - Input C-style string
 * @return Converted LPSTR (must be freed by caller)
 */
inline LPSTR CC_TO_LPSTR(const char* in)
{
	LPSTR out = new char[strlen(in) + 1];
	strcpy_s(out, strlen(in) + 1, in);

	return out;
}

/**
 * Registry access class
 * Provides functionality to query Windows registry values
 */
class c_registry
{
private:
public:
	/**
	 * Default constructor
	 */
	c_registry()
	{
	}

	/**
	 * Default destructor
	 */
	~c_registry()
	{
	}

	/**
	 * Queries a registry value
	 * @param path - Registry path to query
	 * @param type - Expected value type
	 * @return Retrieved value as string
	 */
	std::string QueryValue(const char* path, e_registry_type type);
};
