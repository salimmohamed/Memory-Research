#pragma once
#include "pch.h"

/**
 * @file Overlay.h
 * @brief Overlay configuration settings
 * 
 * This header defines the OverlayConfig class, which manages
 * configuration settings for the cheat overlay window.
 * It provides:
 * - Resolution override options
 * - Window dimensions
 * - JSON serialization/deserialization
 */

/**
 * @brief Overlay configuration class
 * 
 * This class manages configuration settings for the cheat overlay window,
 * including resolution and display settings. It supports JSON serialization
 * for saving and loading configurations.
 */
class OverlayConfig
{
    std::string ConfigName;  ///< Name of this configuration instance

public:
    /**
     * @brief Constructor
     * @param name - Name of the configuration
     * 
     * Initializes a new overlay configuration with the specified name.
     * The name is used for JSON serialization and identification.
     */
    OverlayConfig(const std::string& name)
    {
        ConfigName = name;
    }

    bool OverrideResolution = false;  ///< Whether to override game resolution
    int Width = 2560;                ///< Overlay width in pixels
    int Height = 1440;               ///< Overlay height in pixels

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
     * - Resolution override setting
     * - Window width
     * - Window height
     */
    json ToJson()
    {
        json j;
        j[ConfigName][LIT("OverrideResolution")] = OverrideResolution;
        j[ConfigName][LIT("Width")] = Width;
        j[ConfigName][LIT("Height")] = Height;

        return j;
    }

    /**
     * @brief Loads configuration from JSON format
     * @param j - JSON object containing configuration data
     * 
     * Deserializes configuration settings from JSON format.
     * If a setting is missing in the JSON, its current value
     * is preserved. The function handles:
     * - Resolution override setting
     * - Window width
     * - Window height
     */
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("OverrideResolution")))
            OverrideResolution = j[ConfigName][LIT("OverrideResolution")];
        if (j[ConfigName].contains(LIT("Width")))
            Width = j[ConfigName][LIT("Width")];
        if (j[ConfigName].contains(LIT("Height")))
            Height = j[ConfigName][LIT("Height")];
    }
};

