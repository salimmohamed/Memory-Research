#pragma once
#include "pch.h"
#include "InputManager.h"

/**
 * Memory Class
 * 
 * This class provides Direct Memory Access (DMA) functionality for reading and writing
 * game memory. It serves as the core interface for all memory operations in the cheat.
 * 
 * Key Features:
 * - DMA initialization and management
 * - Process memory reading/writing
 * - Pattern scanning
 * - Scatter read/write operations
 * - Module and export/import table access
 * - Memory dumping and analysis
 */

class Memory
{
private:
	/**
	 * Structure to hold loaded DLL modules
	 * Required for DMA operations
	 */
	struct LibModules
	{
		HMODULE VMM = nullptr;      // Virtual Memory Manager module
		HMODULE FTD3XX = nullptr;   // FTDI USB module for FPGA communication
		HMODULE LEECHCORE = nullptr; // Memory acquisition module
	};

	static inline LibModules modules{ };

	/**
	 * Structure to store current process information
	 * Tracks the target process being accessed
	 */
	struct CurrentProcessInformation
	{
		int PID = 0;                // Process ID
		size_t base_address = 0;    // Base address of the process
		size_t base_size = 0;       // Size of the process memory
		std::string process_name = ""; // Name of the process
	};

	// Map of module names to their base addresses
	std::unordered_map<std::wstring, ULONG64> Modules;

	// State flags for DMA and process initialization
	static inline BOOLEAN DMA_INITIALIZED = FALSE;
	static inline BOOLEAN PROCESS_INITIALIZED = FALSE;

	/**
	 * Dumps the system's physical memory pages to a file
	 * Required for DMA operations to work
	 * @param debug - Enable debug output
	 * @return true if successful
	 */
	bool DumpMemoryMap(bool debug = false);

	/**
	 * Configures the FPGA device for DMA operations
	 * Sets up the hardware for memory access
	 * @return true if successful
	 */
	bool SetFPGA();

	/*this->registry_ptr = std::make_shared<c_registry>(*this);
	this->key_ptr = std::make_shared<c_keys>(*this);*/

public:
	// Current process information
	static inline CurrentProcessInformation current_process{ };

	/**
	 * Constructor and Destructor
	 * Handles initialization and cleanup of DMA resources
	 */
	Memory();
	~Memory();

	/**
	 * Initializes the DMA system
	 * Sets up all required components for memory access
	 * @param process_name - Name of the target process
	 * @param memMap - Whether to dump memory map
	 * @param debug - Enable debug output
	 * @return true if successful
	 */
	bool Init(std::string process_name, bool memMap = true, bool debug = false);

	/*This part here is things related to the process information such as Base daddy, Size ect.*/

	/**
	 * Gets the process ID from process name
	 * @param process_name - Name of the process
	 * @return Process ID
	 */
	DWORD GetPidFromName(std::string process_name);

	/**
	 * Gets all process IDs matching the given name
	 * @param process_name - Name of the process
	 * @return Vector of process IDs
	 */
	std::vector<int> GetPidListFromName(std::string process_name);

	/**
	 * Gets the list of modules loaded by the process
	 * @param process_name - Name of the process
	 * @return Vector of module names
	 */
	std::vector<std::string> GetModuleList(std::string process_name);

	/**
	 * Gets detailed process information
	 * @return Process information structure
	 */
	VMMDLL_PROCESS_INFORMATION GetProcessInformation();


	/**
	 * Gets the base address of a module
	 * @param module_name - Name of the module
	 * @return Base address
	 */
	size_t GetBaseAddress(std::string module_name);

	/**
	 * Gets the size of a module
	 * @param module_name - Name of the module
	 * @return Module size
	 */
	size_t GetBaseSize(std::string module_name);

	/**
	 * Gets the address of an exported function
	 * @param import - Name of the export
	 * @param process - Name of the process
	 * @param module - Name of the module
	 * @return Export address
	 */
	uintptr_t GetExportTableAddress(std::string import, std::string process, std::string module);

	/**
	 * Gets the address of an imported function
	 * @param import - Name of the import
	 * @param process - Name of the process
	 * @param module - Name of the module
	 * @return Import address
	 */
	uintptr_t GetImportTableAddress(std::string import, std::string process, std::string module);

	/**
	 * Fixes CR3 register to bypass anti-cheat protection
	 * Required for reading memory from protected processes
	 * @return true if successful
	 */
	bool FixCr3();

	/**
	 * Dumps process memory to a file
	 * @param address - Starting address
	 * @param path - Output file path
	 * @return true if successful
	 */
	bool DumpMemory(uintptr_t address, std::string path);

	/*This part is where all memory operations are done, such as read, write.*/

	/**
	 * Scans memory for a pattern
	 * @param signature - Pattern to search for (e.g. "48 ? ? ?")
	 * @param range_start - Start address
	 * @param range_end - End address
	 * @param PID - Process ID (optional)
	 * @return Address of found pattern
	 */
	uint64_t FindSignature(const char* signature, uint64_t range_start, uint64_t range_end, int PID = 0);

	/**
	 * Writes memory to the process
	 * @param address - Target address
	 * @param buffer - Data to write
	 * @param size - Size of data
	 * @return true if successful
	 */
	bool Write(uintptr_t address, void* buffer, size_t size) const;
	bool Write(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	 * Template function for writing typed data
	 */
	template <typename T>
	bool Write(void* address, T value)
	{
		return Write(address, &value, sizeof(T));
	}

	template <typename T>
	bool Write(uintptr_t address, T value)
	{
		return Write(address, &value, sizeof(T));
	}

	/**
	 * Reads memory from the process
	 * @param address - Source address
	 * @param buffer - Buffer to store data
	 * @param size - Size to read
	 * @return true if successful
	 */
	bool Read(uintptr_t address, void* buffer, size_t size) const;
	bool Read(uintptr_t address, void* buffer, size_t size, int pid) const;

	/**
	 * Template function for reading typed data
	 */
	template <typename T>
	T Read(void* address)
	{
		T buffer{ };
		memset(&buffer, 0, sizeof(T));
		Read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T));
		return buffer;
	}

	template <typename T>
	T Read(uint64_t address)
	{
		return Read<T>(reinterpret_cast<void*>(address));
	}

	/**
	 * Template function for reading typed data with specific PID
	 */
	template <typename T>
	T Read(void* address, int pid)
	{
		T buffer{ };
		memset(&buffer, 0, sizeof(T));
		Read(reinterpret_cast<uint64_t>(address), reinterpret_cast<void*>(&buffer), sizeof(T), pid);
		return buffer;
	}

	template <typename T>
	T Read(uint64_t address, int pid)
	{
		return Read<T>(reinterpret_cast<void*>(address), pid);
	}

	/**
	 * Creates a scatter handle for batch memory operations
	 * @return Scatter handle
	 */
	VMMDLL_SCATTER_HANDLE CreateScatterHandle();
	VMMDLL_SCATTER_HANDLE CreateScatterHandle(int pid);

	/**
	 * Closes a scatter handle
	 * @param handle - Handle to close
	 */
	void CloseScatterHandle(VMMDLL_SCATTER_HANDLE handle);

	/**
	 * Adds a read request to scatter handle
	 * @param handle - Scatter handle
	 * @param address - Address to read
	 * @param buffer - Buffer to store data
	 * @param size - Size to read
	 * @return true if successful
	 */
	bool AddScatterReadRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);

	/**
	 * Adds a write request to scatter handle
	 * @param handle - Scatter handle
	 * @param address - Address to write
	 * @param buffer - Data to write
	 * @param size - Size to write
	 */
	void AddScatterWriteRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t address, void* buffer, size_t size);
	template <typename T>
	bool AddScatterWriteRequest(VMMDLL_SCATTER_HANDLE handle, uint64_t addr, T value) const
	{
		if (!(addr > 0x2000000 && addr < 0x7FFFFFFFFFFF))
			return false;
		bool ret = !VMMDLL_Scatter_PrepareWrite(handle, addr, reinterpret_cast<PBYTE>(&value), sizeof(value));
		if (!ret)
		{
			LOG("failed to prepare scatter write at 0x%p\n", addr);
		}
		return ret;
	}

	/**
	 * Executes all read requests in scatter handle
	 * @param handle - Scatter handle
	 * @param pid - Process ID (optional)
	 */
	void ExecuteReadScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);

	/**
	 * Executes all write requests in scatter handle
	 * @param handle - Scatter handle
	 * @param pid - Process ID (optional)
	 */
	void ExecuteWriteScatter(VMMDLL_SCATTER_HANDLE handle, int pid = 0);


	/*the FPGA handle*/
	VMM_HANDLE vHandle;
};

inline Memory TargetProcess;
