#pragma once
#include "pch.h"
#include "TabController.h"
#include "../Entities/Entity.h"
#include "Form.h"

class GUI {
public:
    static std::shared_ptr<TabController> Instance;
};

void CreateGUI();
void SetFormPriority();
void Render();
void RenderGUI();
extern bool MenuOpen;
extern int TabCount;
extern D2D1::ColorF ColourPickerClipBoard;
extern int KeyBindClipBoard;
