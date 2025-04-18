#pragma once
#include "pch.h"
#include "ConfigInstance.h"
#include <mutex>
#include <string>

/**
 * @file DebugUtils.h
 * @brief Debug utility functions
 * 
 * This header provides utility functions for debug output and logging.
 */

// Global mutex for thread-safe debug output
extern std::mutex debugMutex;

/**
 * @brief Adds text to the debug output
 * @param text - Text to add to debug output
 * 
 * This function adds text to the debug output with thread safety.
 * It handles:
 * - Thread-safe access to debug text
 * - Auto-scrolling
 * - Line limiting to prevent memory issues
 */
void AddDebugOutput(const std::wstring& text); 