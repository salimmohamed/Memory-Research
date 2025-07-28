#pragma once
#include "../Entities/Window.h"
#include "../Entities/Tab.h"
#include "../Entities/Button.h"
#include "../Entities/Label.h"
#include <vector>
#include <string>

// Maximum number of debug lines to display
const int MAX_DEBUG_LINES = 100;

// Buffer to store debug output
extern std::vector<std::string> DebugOutput;

// Function to add debug output
void AddDebugOutput(const char* text);

class GUI {
public:
    static void CreateGUI();
    static void Render();
    static void Update();
    static void Cleanup();
};

void CreateGUI();
void Render();
extern bool MenuOpen;
extern int TabCount;
extern D2D1::ColorF ColourPickerClipBoard;
extern int KeyBindClipBoard;
