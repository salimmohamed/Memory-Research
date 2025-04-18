#include "pch.h"
#include "CheckBox.h"
#include "Drawing.h"

CheckBox::CheckBox(float x, float y, const std::wstring& text, bool* value)
    : Entity(x, y), m_value(value), m_text(text) {
    m_checkboxRect = D2D1::RectF(x, y, x + 20, y + 20);
    m_textRect = D2D1::RectF(x + 25, y, x + 200, y + 20);
}

void CheckBox::Draw() {
    // Draw checkbox border
    DrawRect(m_checkboxRect, Colour(255, 255, 255, 255), 1.0f);
    
    // Draw checkmark if checked
    if (*m_value) {
        DrawLine(D2D1::Point2F(m_checkboxRect.left + 5, m_checkboxRect.top + 10),
                D2D1::Point2F(m_checkboxRect.left + 8, m_checkboxRect.top + 15),
                Colour(255, 255, 255, 255), 1.0f);
        DrawLine(D2D1::Point2F(m_checkboxRect.left + 8, m_checkboxRect.top + 15),
                D2D1::Point2F(m_checkboxRect.left + 15, m_checkboxRect.top + 5),
                Colour(255, 255, 255, 255), 1.0f);
    }
    
    // Draw text
    DrawText(m_text, m_textRect, Colour(255, 255, 255, 255), 12.0f);
}

void CheckBox::Update() {
    if (IsMouseInRect(m_checkboxRect) && IsMouseClicked()) {
        *m_value = !*m_value;
    }
} 