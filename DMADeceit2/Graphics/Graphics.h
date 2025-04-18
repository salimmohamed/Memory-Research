#pragma once

/**
 * @file Graphics.h
 * @brief Direct2D-based graphics system for cheat overlay
 * 
 * This header defines the core graphics system used for rendering the cheat overlay.
 * It provides a high-performance Direct2D-based rendering system with support for:
 * - Text rendering with custom fonts and caching
 * - Shape drawing (lines, rectangles, circles)
 * - Color management and blending
 * - Window management and composition
 * - Resource caching for optimal performance
 * 
 * The system is designed to minimize CPU/GPU overhead while providing
 * smooth rendering for real-time game overlays.
 */

/**
 * @brief Structure to hold font information and resources
 * 
 * This structure caches font data to optimize text rendering performance.
 * It stores both the font size and the DirectWrite text format object
 * to avoid recreating fonts for frequently used text.
 */
typedef struct FontInformation
{
	size_t size;                    ///< Font size in points
	IDWriteTextFormat* font;        ///< DirectWrite text format object
};

/**
 * @brief Global Direct2D factory instance
 * 
 * Used to create all Direct2D rendering resources.
 * Initialized once during system startup.
 */
extern ID2D1Factory* Factory;

/**
 * @brief Main render target for the overlay window
 * 
 * Handles all drawing operations for the overlay.
 * Created when the overlay window is initialized.
 */
extern ID2D1HwndRenderTarget* RenderTarget;

/**
 * @brief DirectWrite factory for text rendering
 * 
 * Manages text layout and font resources.
 * Used for creating and managing text formats.
 */
extern IDWriteFactory* FontFactory;

/**
 * @brief Default solid color brush for drawing operations
 * 
 * Used as the default brush for all drawing operations.
 * Color can be changed dynamically during rendering.
 */
extern ID2D1SolidColorBrush* Brush;

/**
 * @brief Cache of loaded fonts for efficient text rendering
 * 
 * Maps font names to their corresponding FontInformation structures.
 * Prevents redundant font creation and improves rendering performance.
 */
extern std::map<std::string, FontInformation> Fonts;

/**
 * @brief Cache of text layouts for frequently used strings
 * 
 * Stores pre-computed text layouts to avoid recalculating
 * text metrics and layout information.
 */
extern std::unordered_map<std::wstring, IDWriteTextLayout*> TextLayouts;

/**
 * @brief Additional text layout cache for optimization
 * 
 * Secondary cache for text layouts to further improve
 * rendering performance of common strings.
 */
extern std::map<std::wstring, IDWriteTextLayout*> TextCache;

/**
 * @brief Handle to the overlay window
 * 
 * Window handle used for Direct2D rendering target creation
 * and window management operations.
 */
extern HWND Hwnd;
