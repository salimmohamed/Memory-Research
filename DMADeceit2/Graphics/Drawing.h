#pragma once

/**
 * @file Drawing.h
 * @brief High-level Direct2D drawing interface for the overlay
 * 
 * This header provides a comprehensive set of drawing functions for the cheat overlay.
 * It wraps Direct2D functionality into easy-to-use functions for common drawing operations.
 * The system supports:
 * - Text rendering with custom fonts and alignment
 * - Shape drawing (circles, rectangles, lines)
 * - UI elements (sliders, triangles)
 * - Bitmap rendering and manipulation
 * - Anti-aliased and aliased drawing modes
 * 
 * All drawing operations are optimized for performance and support
 * custom colors through the MyColour structure.
 */

/**
 * @brief Draws text at specified coordinates with custom formatting
 * @param x - X coordinate for text position
 * @param y - Y coordinate for text position
 * @param text - Text string to render
 * @param font - Font name to use
 * @param fontsize - Font size in points
 * @param colour - Text color
 * @param alignment - Text alignment option
 * 
 * This function renders text using the specified font and formatting options.
 * It supports left, center, and right alignment of text.
 * The text is cached for optimal performance on repeated calls.
 */
void DrawText(int x, int y, std::wstring text, std::string font, int fontsize, MyColour colour, FontAlignment alignment);

/**
 * @brief Draws an outlined circle
 * @param x - X coordinate of circle center
 * @param y - Y coordinate of circle center
 * @param radius - Circle radius in pixels
 * @param linewidth - Line thickness in pixels
 * @param colour - Line color
 * 
 * Draws a circle outline with the specified thickness and color.
 * The circle is drawn using anti-aliasing for smooth edges.
 */
void OutlineCircle(int x, int y, float radius, float linewidth, MyColour colour);

/**
 * @brief Draws a filled circle
 * @param x - X coordinate of circle center
 * @param y - Y coordinate of circle center
 * @param radius - Circle radius in pixels
 * @param colour - Fill color
 * 
 * Draws a solid circle filled with the specified color.
 * The circle is drawn using anti-aliasing for smooth edges.
 */
void FilledCircle(int x, int y, float radius, MyColour colour);

/**
 * @brief Draws an outlined rectangle
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Rectangle width in pixels
 * @param height - Rectangle height in pixels
 * @param linewidth - Line thickness in pixels
 * @param colour - Line color
 * 
 * Draws a rectangle outline with the specified thickness and color.
 * The rectangle is drawn using anti-aliasing for smooth edges.
 */
void OutlineRectangle(int x, int y, int width, int height, int linewidth, MyColour colour);

/**
 * @brief Draws a filled rectangle
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Rectangle width in pixels
 * @param height - Rectangle height in pixels
 * @param colour - Fill color
 * 
 * Draws a solid rectangle filled with the specified color.
 * The rectangle is drawn using anti-aliasing for smooth edges.
 */
void FilledRectangle(int x, int y, int width, int height, MyColour colour);

/**
 * @brief Draws an outlined rounded rectangle
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Rectangle width in pixels
 * @param height - Rectangle height in pixels
 * @param linewidth - Line thickness in pixels
 * @param rounding - Corner radius in pixels
 * @param colour - Line color
 * 
 * Draws a rounded rectangle outline with the specified corner radius.
 * The rectangle is drawn using anti-aliasing for smooth edges.
 */
void OutlineRoundedRectangle(int x, int y, int width, int height, int linewidth, int rounding, MyColour colour);

/**
 * @brief Draws a filled rounded rectangle
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Rectangle width in pixels
 * @param height - Rectangle height in pixels
 * @param rounding - Corner radius in pixels
 * @param colour - Fill color
 * 
 * Draws a solid rounded rectangle filled with the specified color.
 * The rectangle is drawn using anti-aliasing for smooth edges.
 */
void FilledRoundedRectangle(int x, int y, int width, int height, int rounding, MyColour colour);

/**
 * @brief Draws a filled line with anti-aliasing
 * @param xstart - X coordinate of start point
 * @param ystart - Y coordinate of start point
 * @param xend - X coordinate of end point
 * @param yend - Y coordinate of end point
 * @param width - Line thickness in pixels
 * @param colour - Line color
 * 
 * Draws a smooth line between two points using anti-aliasing.
 * The line is drawn with the specified thickness and color.
 */
void FilledLine(int xstart, int ystart, int xend, int yend, int width, MyColour colour);

/**
 * @brief Draws a filled line without anti-aliasing
 * @param xstart - X coordinate of start point
 * @param ystart - Y coordinate of start point
 * @param xend - X coordinate of end point
 * @param yend - Y coordinate of end point
 * @param width - Line thickness in pixels
 * @param colour - Line color
 * 
 * Draws a sharp line between two points without anti-aliasing.
 * Useful for pixel-perfect rendering or when performance is critical.
 */
void FilledLineAliased(int xstart, int ystart, int xend, int yend, int width, MyColour colour);

/**
 * @brief Draws a bitmap at specified coordinates with custom dimensions
 * @param bmp - Bitmap to render
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Display width in pixels
 * @param height - Display height in pixels
 * 
 * Renders a bitmap at the specified location with custom dimensions.
 * The bitmap is scaled to fit the specified width and height.
 */
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y, int width, int height);

/**
 * @brief Draws a bitmap at specified coordinates with original dimensions
 * @param bmp - Bitmap to render
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * 
 * Renders a bitmap at the specified location using its original dimensions.
 * The bitmap is not scaled or transformed.
 */
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y);

/**
 * @brief Draws a color saturation slider
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Slider width in pixels
 * @param height - Slider height in pixels
 * @param colour - Base color for the slider
 * 
 * Renders a color saturation slider control.
 * The slider shows a gradient from the base color to its desaturated version.
 */
void SaturationSlider(int x, int y, int width, int height, MyColour colour);

/**
 * @brief Draws a color alpha (transparency) slider
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Slider width in pixels
 * @param height - Slider height in pixels
 * @param col - Base color for the slider
 * 
 * Renders a color transparency slider control.
 * The slider shows a gradient from fully opaque to fully transparent.
 */
void AlphaSlider(int x, int y, int width, int height, MyColour col);

/**
 * @brief Draws a color hue slider
 * @param x - X coordinate of top-left corner
 * @param y - Y coordinate of top-left corner
 * @param width - Slider width in pixels
 * @param height - Slider height in pixels
 * 
 * Renders a color hue slider control.
 * The slider shows a gradient of all possible hue values.
 */
void HueSlider(int x, int y, int width, int height);

/**
 * @brief Draws a filled triangle
 * @param x1 - X coordinate of first vertex
 * @param y1 - Y coordinate of first vertex
 * @param x2 - X coordinate of second vertex
 * @param y2 - Y coordinate of second vertex
 * @param x3 - X coordinate of third vertex
 * @param y3 - Y coordinate of third vertex
 * @param colour - Fill color
 * 
 * Draws a solid triangle defined by three vertices.
 * The triangle is filled with the specified color and uses anti-aliasing.
 */
void FilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, MyColour colour);
