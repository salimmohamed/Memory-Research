#include "pch.h"
#include "GUI.h"
#include "entity.h"
#include "Form.h"
#include "Button.h"
#include "ColourPicker.h"
#include "Label.h"
#include "tab.h"
#include "TabController.h"
#include "Toggle.h"
#include "Slider.h"
#include "DropDown.h"
#include "ComboBox.h"
#include "KeyBind.h"
#include "TabListBox.h"
#include "TabListBoxController.h"
#include "TextBox.h"
#include "ConfigUtilities.h"

// Global variables for GUI state management
int SelectedTab = 1;        // Currently selected tab index
int SelectedSubTab = 0;     // Currently selected sub-tab index
int TabCount = 0;          // Counter for total number of tabs
int KeyBindClipBoard = 0;   // Clipboard for key bindings
EntityVector MenuEntity;    // Main container for all GUI elements
bool MenuOpen = true;      // Flag to control menu visibility

// Color settings for the GUI
D2D1::ColorF ColourPickerClipBoard = Colour(255,255,255);  // Default color for color picker
D2D1::ColorF ColourPick = Colour(0, 150, 255, 255);        // Highlight color

// Screen resolution settings
std::wstring ScreenWidth = std::to_wstring(Configs.Overlay.Width);
std::wstring ScreenHeight = std::to_wstring(Configs.Overlay.Height);

// Main function to create and initialize the GUI
void CreateGUI()
{
	// Create main container for all GUI elements
	MenuEntity = std::make_shared<Container>();
	
	// Create main form window
	auto form = std::make_shared<Form>(100, 100.0f, 300, 200, 2, 30, LIT(L"DMA Deceit 2"), false);
	{
		// Create tab controller to manage all tabs
		auto tabcontroller = std::make_shared<TabController>();
		form->Push(tabcontroller);

		// Create Player ESP Tab
		auto playeresptab = std::make_shared<Tab>(LIT(L"Player ESP"), 5, 5, &SelectedTab, 0, 20);
		{
			// Name toggle with color picker
			auto name = std::make_shared<Toggle>(100, 5, LIT(L"Name"), &Configs.Survivor.Name);
			playeresptab->Push(name);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Survivor.TextColour);
			playeresptab->Push(textcolour);

			// Distance settings
			auto distance = std::make_shared<Toggle>(100, 25, LIT(L"Distance"), &Configs.Survivor.Distance);
			playeresptab->Push(distance);
			auto maxdistance = std::make_shared<Slider<int>>(100, 45, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Survivor.MaxDistance);
			playeresptab->Push(maxdistance);

			// Text size settings
			auto textsize = std::make_shared<Slider<int>>(100, 70, 150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Survivor.FontSize);
			playeresptab->Push(textsize);
		}
		tabcontroller->Push(playeresptab);

		// Create Infected ESP Tab
		auto killeresptab = std::make_shared<Tab>(LIT(L"Infected ESP"), 5, 30, &SelectedTab, 0, 20);
		{
			// Name toggle with color picker
			auto name = std::make_shared<Toggle>(100, 5, LIT(L"Name"), &Configs.Killer.Name);
			killeresptab->Push(name);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Killer.TextColour);
			killeresptab->Push(textcolour);

			// Distance settings
			auto distance = std::make_shared<Toggle>(100, 25, LIT(L"Distance"), &Configs.Killer.Distance);
			killeresptab->Push(distance);
			auto maxdistance = std::make_shared<Slider<int>>(100, 45, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Killer.MaxDistance);
			killeresptab->Push(maxdistance);

			// Text size settings
			auto textsize = std::make_shared<Slider<int>>(100, 70, 150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Killer.FontSize);
			killeresptab->Push(textsize);
		}
		tabcontroller->Push(killeresptab);

		// Create Overlay Tab
		auto overlaytab = std::make_shared<Tab>(LIT(L"Overlay"), 5, 55, &SelectedTab, 0, 20);
		{
			// Resolution override toggle
			auto overrideresolution = std::make_shared<Toggle>(100, 5, LIT(L"Override W2S Resolution"), &Configs.Overlay.OverrideResolution);
			overlaytab->Push(overrideresolution);

			// Screen width input with validation
			auto screenwidth = std::make_shared<TextBox>(100, 35, LIT(L"Screen Width"), &ScreenWidth);
			screenwidth->SetValueChangedEvent([]()
				{
					try
					{
						Configs.Overlay.Width = std::stoi(ScreenWidth);
					}
					catch (std::exception ex)
					{
					}
				});
			overlaytab->Push(screenwidth);

			// Screen height input with validation
			auto screenheight = std::make_shared<TextBox>(100, 70, LIT(L"Screen Height"), &ScreenHeight);
			screenheight->SetValueChangedEvent([]()
				{
					try
					{
						Configs.Overlay.Height = std::stoi(ScreenHeight);
					}
					catch (std::exception ex)
					{
					}
				});
			overlaytab->Push(screenheight);
		}
		tabcontroller->Push(overlaytab);

		// Create Config Tab
		auto configtab = std::make_shared<Tab>(LIT(L"Config"), 5, 80, &SelectedTab, 0, 20);
		{
			// Save configuration button
			auto saveconfig = std::make_shared<Button>(100, 5, LIT(L"Save"), []()
				{
					SaveConfig(L"Default.json");
					CreateGUI(); // reinit/ reload
					SelectedTab = 1;
				});
			configtab->Push(saveconfig);

			// Load configuration button
			auto loadconfig = std::make_shared<Button>(165, 5, LIT(L"Load"), []()
				{
					LoadConfig(L"Default.json");
					CreateGUI(); // reinit/ reload
					SelectedTab = 1;
				});
			configtab->Push(loadconfig);
		}
		tabcontroller->Push(configtab);
	}

	// Add form to main container and initialize
	MenuEntity->Push(form);
	MenuEntity->Draw();
	MenuEntity->Update();
}

// Function to handle form z-order (which form appears on top)
void SetFormPriority()
{
	// Sort forms based on last click time to determine which should be on top
	std::sort(MenuEntity->GetContainer().begin(), MenuEntity->GetContainer().end(),
	          [](child a, child b) { return b->GetLastClick() < a->GetLastClick(); }
	);
}

// Variable to track menu open/close timing
float LastOpen = 0;

// Main render function for the GUI
void Render()
{
	// Toggle menu visibility with INSERT key
	if (IsKeyClicked(VK_INSERT) && LastOpen < clock() * 0.00001f)
	{
		LastOpen = (clock() * 0.00001f) + 0.002f;
		MenuOpen = !MenuOpen;
	}

	// Draw and update GUI elements
	MenuEntity->Draw();
	MenuEntity->GetContainer()[0]->Update(); // Only allow main form updates
	SetFormPriority();
}
