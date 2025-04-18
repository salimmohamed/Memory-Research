#include "pch.h"
#include "DebugUtils.h"
#include "Globals.h"  // For Configs
#include <mutex>

// Global mutex for thread-safe debug output
std::mutex debugMutex;

void AddDebugOutput(const std::wstring& text) {
    std::lock_guard<std::mutex> lock(debugMutex);
    if (Configs.Debug.AutoScroll) {
        // Add new line if not empty
        if (!Configs.Debug.Info.empty()) {
            Configs.Debug.Info += L"\n";
        }
        Configs.Debug.Info += text;

        // Keep only the last 1000 lines to prevent excessive memory usage
        size_t maxLines = 1000;
        size_t lineCount = 0;
        size_t pos = 0;
        std::wstring::size_type lastPos = 0;

        // Count lines and find position to truncate
        while ((pos = Configs.Debug.Info.find(L'\n', lastPos)) != std::wstring::npos) {
            lineCount++;
            if (lineCount > maxLines) {
                Configs.Debug.Info = Configs.Debug.Info.substr(lastPos + 1);
                break;
            }
            lastPos = pos;
        }
    }
} 