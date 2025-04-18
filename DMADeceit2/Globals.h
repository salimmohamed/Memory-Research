#pragma once
#include "Engine.h"

/**
 * @file Globals.h
 * @brief Global variables and instances
 * 
 * This header defines global variables used throughout the cheat.
 * It provides access to:
 * - Engine instance
 * - Process information
 * 
 * These globals are used to maintain state and provide access
 * to core functionality across different components of the cheat.
 */

/**
 * @brief Global engine instance
 * 
 * This variable holds the main engine instance used by the cheat.
 * It provides access to:
 * - Game memory reading/writing
 * - Actor management
 * - Camera information
 * - Game state tracking
 */
extern std::shared_ptr<Engine> EngineInstance;

/**
 * @brief Target process name
 * 
 * This variable stores the name of the target process that
 * the cheat is attached to. It is used for:
 * - Process identification
 * - Memory access
 * - Initialization checks
 */
extern std::string ProcessName;