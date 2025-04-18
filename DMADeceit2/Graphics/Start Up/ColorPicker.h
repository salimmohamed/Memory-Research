#pragma once
#include "pch.h"
#include "entity.h"
#include "Label.h"

class ColorPicker : public Entity {
public:
    ColorPicker(float x, float y, const std::wstring& text, D2D1::ColorF* value);
    void Draw() override;
    void Update() override;

private:
    D2D1::ColorF* m_value;
    std::wstring m_text;
    D2D1_RECT_F m_colorRect;
    D2D1_RECT_F m_textRect;
}; 