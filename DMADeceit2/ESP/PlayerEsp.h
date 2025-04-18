#pragma once

/**
 * @file PlayerEsp.h
 * @brief Player ESP (Extra Sensory Perception) system interface
 * 
 * This header defines the interface for the Player ESP system, which provides
 * visual information about players in the game world. The system consists of
 * two main components:
 * 
 * 1. Data Collection:
 *    - Reads player data from game memory using DMA
 *    - Processes and filters player information
 *    - Updates player states and positions
 * 
 * 2. Visual Rendering:
 *    - Draws player information on screen
 *    - Shows player positions, health, and other data
 *    - Provides visual cues for player states
 * 
 * The ESP system is designed to be efficient and customizable, with support for:
 * - Different ESP styles and configurations
 * - Real-time updates of player information
 * - Performance optimization for minimal impact on game performance
 */

/**
 * @brief Global function pointer for player data updates
 * 
 * This function pointer is used to periodically update player information
 * from game memory. It is called at regular intervals to ensure the ESP
 * displays current player data.
 * 
 * The function:
 * - Reads player data from game memory
 * - Updates player positions and states
 * - Filters out invalid or irrelevant players
 * - Maintains a cache of player information
 */
extern std::shared_ptr<CheatFunction> UpdatePlayers;

/**
 * @brief Main function for rendering player ESP elements
 * 
 * This function is called every frame to draw ESP information on screen.
 * It handles:
 * - Converting 3D player positions to 2D screen coordinates
 * - Drawing player information (health, distance, etc.)
 * - Applying visual styles and configurations
 * - Managing ESP element visibility and updates
 * 
 * The function uses the Direct2D drawing system to render ESP elements
 * with optimal performance and visual quality.
 */
extern void DrawPlayerEsp();