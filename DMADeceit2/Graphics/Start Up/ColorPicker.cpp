#include "pch.h"
#include "ColorPicker.h"
#include "Drawing.h"

ColorPicker::ColorPicker(float x, float y, const std::wstring& text, D2D1::ColorF* value)
    : Entity(x, y), m_value(value), m_text(text) {
    m_colorRect = D2D1::RectF(x, y, x + 20, y + 20);
    m_textRect = D2D1::RectF(x + 25, y, x + 200, y + 20);
}

void ColorPicker::Draw() {
    // Draw color preview
    FillRect(m_colorRect, *m_value);
    DrawRect(m_colorRect, Colour(255, 255, 255, 255), 1.0f);
    
    // Draw text
    DrawText(m_text, m_textRect, Colour(255, 255, 255, 255), 12.0f);
}

void ColorPicker::Update() {
    if (IsMouseInRect(m_colorRect) && IsMouseClicked()) {
        // TODO: Implement color picker dialog
        // For now, just cycle through some basic colors
        static const D2D1::ColorF colors[] = {
            Colour(255, 0, 0, 255),    // Red
            Colour(0, 255, 0, 255),    // Green
            Colour(0, 0, 255, 255),    // Blue
            Colour(255, 255, 0, 255),  // Yellow
            Colour(255, 0, 255, 255),  // Magenta
            Colour(0, 255, 255, 255)   // Cyan
        };
        static int colorIndex = 0;
        *m_value = colors[colorIndex];
        colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
    }
} 