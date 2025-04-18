#pragma once
#include "pch.h"
#include "entity.h"
#include "Label.h"

class CheckBox : public Entity {
public:
    CheckBox(float x, float y, const std::wstring& text, bool* value);
    void Draw() override;
    void Update() override;

private:
    bool* m_value;
    std::wstring m_text;
    D2D1_RECT_F m_checkboxRect;
    D2D1_RECT_F m_textRect;
}; 