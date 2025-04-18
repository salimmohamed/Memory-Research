#pragma once
#include "pch.h"

/**
 * @file DebugConfig.h
 * @brief Debug configuration settings
 * 
 * This header defines the DebugConfig class, which manages
 * configuration settings for debug information display.
 * It provides:
 * - Debug information text storage
 * - JSON serialization/deserialization
 */

/**
 * @brief Debug configuration class
 * 
 * This class manages configuration settings for debug information display,
 * including text storage and formatting. It supports JSON serialization
 * for saving and loading configurations.
 */
class DebugConfig
{
    std::string ConfigName;  ///< Name of this configuration instance

public:
    /**
     * @brief Constructor
     * @param name - Name of the configuration
     * 
     * Initializes a new debug configuration with the specified name.
     * The name is used for JSON serialization and identification.
     */
    DebugConfig(const std::string& name)
    {
        ConfigName = name;
    }

    std::wstring Info;  ///< Debug information text
    bool AutoScroll = true;  ///< Whether to auto-scroll to bottom
    float FontSize = 11.0f;  ///< Font size for debug text
    MyColour TextColor = MyColour(255, 255, 255, 255);  ///< Text color

    DebugConfig() = default;

    /**
     * @brief Converts configuration to JSON format
     * @return JSON object containing configuration data
     * 
     * Serializes all configuration settings to JSON format,
     * including:
     * - Debug information text
     * - Auto-scroll setting
     * - Font size
     * - Text color
     */
    json ToJson() const
    {
        json j;
        j[ConfigName]["Info"] = std::string(Info.begin(), Info.end());
        j[ConfigName]["AutoScroll"] = AutoScroll;
        j[ConfigName]["FontSize"] = FontSize;
        j[ConfigName]["TextColor"] = {
            {"r", TextColor.r},
            {"g", TextColor.g},
            {"b", TextColor.b},
            {"a", TextColor.a}
        };
        return j;
    }

    /**
     * @brief Loads configuration from JSON format
     * @param j - JSON object containing configuration data
     * 
     * Deserializes configuration settings from JSON format.
     * If a setting is missing in the JSON, its current value
     * is preserved. The function handles:
     * - Debug information text
     * - Auto-scroll setting
     * - Font size
     * - Text color
     */
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains("Info"))
            Info = std::wstring(j[ConfigName]["Info"].get<std::string>().begin(), j[ConfigName]["Info"].get<std::string>().end());
        if (j[ConfigName].contains("AutoScroll"))
            AutoScroll = j[ConfigName]["AutoScroll"].get<bool>();
        if (j[ConfigName].contains("FontSize"))
            FontSize = j[ConfigName]["FontSize"].get<float>();
        if (j[ConfigName].contains("TextColor")) {
            TextColor.r = j[ConfigName]["TextColor"]["r"].get<float>();
            TextColor.g = j[ConfigName]["TextColor"]["g"].get<float>();
            TextColor.b = j[ConfigName]["TextColor"]["b"].get<float>();
            TextColor.a = j[ConfigName]["TextColor"]["a"].get<float>();
        }
    }
}; 