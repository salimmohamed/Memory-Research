#include "Pch.h"
#include "CheatFunction.h"
#include "Globals.h"
#include <mutex>
#include "DebugUtils.h"

/**
 * @file CheatFunction.cpp
 * @brief Implementation of the cheat function management system
 * 
 * This file implements the CheatFunction class, providing the actual
 * functionality for interval-based function execution.
 */

/**
 * @brief Constructor implementation
 * @param time - Time interval between executions in milliseconds
 * @param func - Function to execute
 * 
 * Initializes the cheat function with:
 * - Execution interval
 * - Function to execute
 * - Initial execution timestamp
 */
CheatFunction::CheatFunction(int time, std::function<void()> func)
{
	MsSleep = time;
	Function = func;
	LastExecution = GetTickCount64();
}

/**
 * @brief Execute function implementation
 * 
 * This function:
 * 1. Checks if enough time has passed since last execution
 * 2. Executes the function if interval has elapsed
 * 3. Updates the last execution timestamp
 * 
 * The function uses GetTickCount64() for timing to ensure
 * accurate interval control and prevent performance issues.
 */
void CheatFunction::Execute()
{
	if (GetTickCount64() - LastExecution > MsSleep)
	{
		Function();
		LastExecution = GetTickCount64();
	}
}