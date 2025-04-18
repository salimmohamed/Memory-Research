#pragma once
#include "ConfigInstance.h"

/**
 * @file ConfigUtilities.h
 * @brief Configuration management utilities
 * 
 * This header provides functions for managing cheat configurations,
 * including saving, loading, and enumerating configuration files.
 * The configuration system supports:
 * - Multiple named configurations
 * - Persistent storage of settings
 * - Runtime configuration updates
 * - Default configuration setup
 */

/**
 * @brief Saves the current configuration to a file
 * @param config - Name of the configuration to save
 * 
 * This function serializes the current configuration state
 * and saves it to a file with the specified name.
 * The configuration is stored in a format that can be
 * loaded later using LoadConfig.
 */
void SaveConfig(const std::wstring& config);

/**
 * @brief Loads a configuration from a file
 * @param config - Name of the configuration to load
 * 
 * This function reads a configuration file and applies
 * its settings to the current configuration state.
 * If the specified configuration doesn't exist, the
 * default configuration is loaded instead.
 */
void LoadConfig(const std::wstring& config);

/**
 * @brief Gets a list of all available configurations
 * @return Vector of configuration names
 * 
 * This function enumerates all saved configuration files
 * and returns their names. The names can be used with
 * SaveConfig and LoadConfig to manage configurations.
 */
std::vector<std::wstring> GetAllConfigs();

/**
 * @brief Global configuration instances
 * 
 * This variable holds all configuration instances used by the cheat.
 * It includes settings for:
 * - ESP configuration
 * - Visual settings
 * - Performance options
 * - Other cheat features
 */
extern ConfigInstances Configs;

/**
 * @brief Sets up the default configuration
 * 
 * This function initializes the configuration system with
 * default values for all settings. It is called during
 * cheat initialization to ensure a valid configuration
 * state exists before any features are enabled.
 */
extern void SetUpConfig();