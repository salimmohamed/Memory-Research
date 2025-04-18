#pragma once
#include "pch.h"

/**
 * @file PlayerConfig.h
 * @brief Player-specific configuration settings
 * 
 * This header defines the PlayerConfig class, which manages
 * configuration settings specific to player visualization.
 * It provides:
 * - Text display options
 * - Distance settings
 * - Color customization
 * - Font configuration
 * - JSON serialization/deserialization
 */

/**
 * @brief Player configuration class
 * 
 * This class manages configuration settings for player visualization,
 * including text display, distance settings, and visual styling.
 * It supports JSON serialization for saving and loading configurations.
 */
class PlayerConfig
{
	std::string ConfigName;  ///< Name of this configuration instance

public:
	/**
	 * @brief Constructor
	 * @param name - Name of the configuration
	 * 
	 * Initializes a new player configuration with the specified name.
	 * The name is used for JSON serialization and identification.
	 */
	PlayerConfig(const std::string& name)
	{
		ConfigName = name;
	}
    bool Name = true;
    bool Distance = true;
	int MaxDistance = 1000;
	D2D1::ColorF TextColour = Colour(255, 255, 255);
	int FontSize = 11;
    /**
     * @brief Serializes a color to JSON
     * @param j - JSON object to write to
     * @param name - Name of the color property
     * @param colour - Color to serialize
     * 
     * Converts a D2D1::ColorF to JSON format, storing the
     * red, green, blue, and alpha components.
     */
    void ToJsonColour(json* j, const std::string& name, D2D1::ColorF* colour)
    {
        (*j)[ConfigName][name][LIT("r")] = colour->r;
        (*j)[ConfigName][name][LIT("g")] = colour->g;
        (*j)[ConfigName][name][LIT("b")] = colour->b;
        (*j)[ConfigName][name][LIT("a")] = colour->a;

    }
    /**
     * @brief Deserializes a color from JSON
     * @param j - JSON object to read from
     * @param name - Name of the color property
     * @param colour - Color to update
     * 
     * Converts JSON color data to D2D1::ColorF format.
     * If the color property doesn't exist in the JSON,
     * the color remains unchanged.
     */
    void FromJsonColour(json j, const std::string& name, D2D1::ColorF* colour)
    {
        if (j[ConfigName].contains(name))
        {
            colour->r = j[ConfigName][name][LIT("r")];
            colour->g = j[ConfigName][name][LIT("g")];
            colour->b = j[ConfigName][name][LIT("b")];
            colour->a = j[ConfigName][name][LIT("a")];
        }
    }

    /**
     * @brief Converts configuration to JSON format
     * @return JSON object containing configuration data
     * 
     * Serializes all configuration settings to JSON format,
     * including:
     * - Name display setting
     * - Distance display setting
     * - Maximum distance
     * - Font size
     * - Text color
     */
    json ToJson()
    {
        json j;
        j[ConfigName][LIT("Name")] = Name;
        j[ConfigName][LIT("FontSize")] = FontSize;
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        ToJsonColour(&j, LIT("TextColour"), &TextColour);

        return j;
    }
    /**
     * @brief Loads configuration from JSON format
     * @param j - JSON object containing configuration data
     * 
     * Deserializes configuration settings from JSON format.
     * If a setting is missing in the JSON, its current value
     * is preserved. The function handles:
     * - Name display setting
     * - Distance display setting
     * - Maximum distance
     * - Font size
     * - Text color
     */
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("Name")))
            Name = j[ConfigName][LIT("Name")];
        if (j[ConfigName].contains(LIT("Distance")))
            Distance = j[ConfigName][LIT("Distance")];
        if (j[ConfigName].contains(LIT("FontSize")))
            FontSize = j[ConfigName][LIT("FontSize")];
        if (j[ConfigName].contains(LIT("MaxDistance")))
            MaxDistance = j[ConfigName][LIT("MaxDistance")];
        FromJsonColour(j, LIT("TextColour"), &TextColour);
    }
};

