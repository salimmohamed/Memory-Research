#pragma once
#include "Pch.h"
#include "PlayerConfig.h"
#include "Overlay.h"
#include "DebugConfig.h"

/**
 * @file ConfigInstance.h
 * @brief Configuration instance management
 * 
 * This header defines the ConfigInstances class, which manages
 * all configuration instances used by the cheat. It provides:
 * - Configuration storage for different player types
 * - Overlay configuration management
 * - Debug configuration management
 * - JSON serialization/deserialization
 * - Default configuration values
 */

/**
 * @brief Configuration instances container class
 * 
 * This class manages all configuration instances used by the cheat.
 * It provides access to:
 * - Survivor player configuration
 * - Killer player configuration
 * - Overlay configuration
 * - Debug configuration
 * 
 * The class also handles serialization and deserialization of
 * configurations to/from JSON format.
 */
class ConfigInstances
{
public:
	PlayerConfig Survivor = PlayerConfig(LIT("Survivor"));  ///< Configuration for survivor players
	PlayerConfig Killer = PlayerConfig(LIT("Killer"));      ///< Configuration for killer players
	OverlayConfig Overlay = OverlayConfig(LIT("Overlay"));  ///< Overlay configuration
	DebugConfig Debug = DebugConfig(LIT("Debug"));          ///< Debug configuration

	/**
	 * @brief Default constructor
	 * 
	 * Initializes configuration instances with default values:
	 * - Survivor text color: Light blue (110, 169, 251)
	 * - Killer text color: Red (212, 72, 72)
	 */
	ConfigInstances()
	{
		Survivor.TextColour = Colour(110, 169, 251); // Light blue for survivors
		Killer.TextColour = Colour(212, 72, 72); // Red for killers
	}

	/**
	 * @brief Converts configuration to JSON format
	 * @return JSON object containing all configuration data
	 * 
	 * This function serializes all configuration instances
	 * into a single JSON object. The JSON structure includes:
	 * - Survivor configuration
	 * - Killer configuration
	 * - Overlay configuration
	 * - Debug configuration
	 */
	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Survivor.ToJson());
		jsoned.merge_patch(Overlay.ToJson());
		jsoned.merge_patch(Killer.ToJson());
		jsoned.merge_patch(Debug.ToJson());
		return jsoned;
	}

	/**
	 * @brief Loads configuration from JSON format
	 * @param jsoned - JSON object containing configuration data
	 * 
	 * This function deserializes configuration data from a JSON object.
	 * It updates all configuration instances with the values from the JSON.
	 * If a configuration value is missing in the JSON, the default value
	 * is preserved.
	 */
	void FromJson(json jsoned)
	{
		Survivor.FromJson(jsoned);
		Overlay.FromJson(jsoned);
		Killer.FromJson(jsoned);
		Debug.FromJson(jsoned);
	}
};