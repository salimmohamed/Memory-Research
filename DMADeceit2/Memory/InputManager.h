#pragma once
#include "pch.h"
#include "Registry.h"

/**
 * Keyboard input handling namespace
 * Provides functionality to read keyboard state using DMA
 */
namespace Keyboard
{
	/**
	 * Initializes keyboard state tracking
	 * Sets up DMA access to keyboard state in win32kbase.sys
	 * @return true if initialization successful
	 */
	bool InitKeyboard();

	/**
	 * Updates the current keyboard state
	 * Reads keyboard state from kernel memory
	 */
	void UpdateKeys();

	/**
	 * Checks if a key is currently pressed
	 * @param virtual_key_code - Windows virtual key code to check
	 * @return true if key is pressed
	 */
	bool IsKeyDown(uint32_t virtual_key_code);
};
