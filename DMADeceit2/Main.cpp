#include "pch.h"
#include "Memory.h"
#include "CheatFunction.h"
#include "Globals.h"
#include "Init.h"
#include "GUI.h"
#include "Engine.h"

/**
 * @file Main.cpp
 * @brief Main application entry point and window management
 * 
 * This file contains the main entry point for the cheat application
 * and handles window creation, message processing, and initialization.
 * It provides:
 * - Process attachment and initialization
 * - Window creation and management
 * - Message loop and rendering
 * - Cleanup and shutdown
 */

// Global engine instance for managing game state and features
std::shared_ptr<Engine> EngineInstance;
// Name of the target process
std::string ProcessName;

/**
 * @brief Main initialization function
 * 
 * This function sets up the cheat by:
 * 1. Finding and attaching to the target game process
 * 2. Initializing DMA communication
 * 3. Setting up the engine and caching game data
 * 
 * It supports multiple game processes:
 * - Deceit2Game-Win64-Shipping.exe
 * - DeadByDaylight-EGS-Shipping.exe
 * 
 * The function exits if it cannot find a valid game process.
 */
void main()
{
	bool gamefound = true;
	ProcessName = "Deceit2Game-Win64-Shipping.exe";

	std::wstringstream ss;
	if (TargetProcess.Init("Deceit2Game-Win64-Shipping.exe"))
	{
		ProcessName = "Deceit2Game-Win64-Shipping.exe";
	}
	else if (TargetProcess.Init("DeadByDaylight-EGS-Shipping.exe"))
	{
		ProcessName = "DeadByDaylight-EGS-Shipping.exe";
	}
	else
	{
		ss << L"Failed to find game\n";
		AddDebugOutput(ss.str());
		return;
	}
	TargetProcess.FixCr3();

	uint64_t base = TargetProcess.GetBaseAddress(ProcessName);
	uint64_t size = TargetProcess.GetBaseSize(ProcessName);
	EngineInstance = std::make_shared<Engine>();
	EngineInstance->Cache();

	//uint64_t persistentlevel = 0x190;
	//persistentlevel = TargetProcess.Read<uint64_t>(gobjects + gameinstance);
}

/**
 * @brief Window procedure for handling window messages
 * @param hWnd - Window handle
 * @param message - Windows message
 * @param wParam - Additional message information
 * @param lParam - Additional message information
 * @return Result of message processing
 * 
 * This function processes window messages for the overlay window.
 * It handles:
 * - Window destruction
 * - Input events
 * - Other window messages
 */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InputWndProc(hWnd, message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
 * @brief Entry point for the Windows application
 * @param hInstance - Application instance handle
 * @param hPrevInstance - Previous instance handle (unused)
 * @param lpCmdLine - Command line arguments
 * @param nCmdShow - Window show command
 * @return Application exit code
 * 
 * This function:
 * 1. Sets up the console window for debugging
 * 2. Creates the main overlay window
 * 3. Initializes Direct2D and GUI
 * 4. Enters the message loop
 * 5. Handles cleanup on exit
 * 
 * The message loop processes:
 * - Window messages
 * - Input events
 * - Rendering updates
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	std::wstringstream ss;
	ss << L"Debugging Window:\n";
	AddDebugOutput(ss.str());

	main();
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GUI Framework";
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, L"GUI Framework",
		WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return -1;

	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	ShowWindow(hWnd, nCmdShow);

	InitD2D(hWnd);
	CreateGUI();
	MSG msg;
	SetProcessDPIAware();
	SetInput();
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		RenderFrame();
	}
	CleanD2D();
	return msg.wParam;
}
