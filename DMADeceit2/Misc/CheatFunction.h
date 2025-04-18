#pragma once

/**
 * @file CheatFunction.h
 * @brief Cheat function management system
 * 
 * This header defines the CheatFunction class, which provides a mechanism
 * for executing functions at specified intervals. It is used to:
 * - Schedule periodic updates
 * - Control function execution timing
 * - Manage performance-critical operations
 */

/**
 * @brief Cheat function management class
 * 
 * This class manages the execution of functions at specified intervals.
 * It provides:
 * - Interval-based execution control
 * - Performance optimization
 * - Function scheduling
 */
class CheatFunction
{
	int MsSleep = 0;                ///< Time interval between executions in milliseconds
	std::function<void()> Function; ///< Function to execute
	int LastExecution;              ///< Timestamp of last execution

public:
	/**
	 * @brief Constructor
	 * @param time - Time interval between executions in milliseconds
	 * @param func - Function to execute
	 * 
	 * Initializes a new cheat function with the specified interval and function.
	 * The function will be executed every 'time' milliseconds.
	 */
	CheatFunction(int time, std::function<void()>func);

	/**
	 * @brief Executes the function if enough time has passed
	 * 
	 * Checks if the specified time interval has elapsed since the last execution.
	 * If so, executes the function and updates the last execution timestamp.
	 */
	void Execute();
};