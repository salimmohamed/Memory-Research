#include "Pch.h"
#include "Drawing.h"
#include "Globals.h"
#include "Camera.h"
#include "ActorEntity.h"
#include "Engine.h"
#include "PlayerEsp.h"
#include "ConfigUtilities.h"

/**
 * @brief Utility function to check if a string contains only Latin characters
 * @param str - The string to check
 * @return true if all characters are Latin, false otherwise
 * 
 * This function is used to filter out non-Latin player names that might
 * cause rendering issues with the Direct2D text rendering system.
 * It checks each character in the string to ensure it falls within the
 * basic Latin range (0x0000 to 0x007F).
 */
bool IsLatinString(const std::wstring& str) {
	for (wchar_t c : str) {
		// Check if character is outside Latin range
		if (c > 0x007F) { // 0x007F is the end of basic Latin
			return false;
		}
	}
	return true;
}

/**
 * @brief Global function for updating player data
 * 
 * This function is called every 5 milliseconds to refresh player information.
 * It delegates the actual update to the Engine class, which handles:
 * - Reading player data from game memory
 * - Updating player positions and states
 * - Filtering out invalid or irrelevant players
 * - Maintaining the player cache
 */
std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(5, [] {
	if (!EngineInstance)
		return;
	EngineInstance->UpdatePlayers();
	});

/**
 * @brief Main ESP rendering function
 * 
 * This function is called every frame to draw ESP information for all players.
 * It performs the following operations:
 * 
 * 1. Actor Processing:
 *    - Iterates through all actors in the game
 *    - Filters out invalid or irrelevant actors
 *    - Determines player role (Survivor/Killer)
 * 
 * 2. Position Calculation:
 *    - Converts 3D world positions to 2D screen coordinates
 *    - Calculates distance from camera to player
 *    - Applies distance-based filtering
 * 
 * 3. Information Rendering:
 *    - Draws player names and character names
 *    - Displays distance information
 *    - Applies configuration-based styling
 * 
 * The function uses the Direct2D drawing system to render ESP elements
 * with optimal performance and visual quality.
 */
void DrawPlayerEsp()
{
	if (!EngineInstance)
		return;
	for (auto entity : EngineInstance->GetActors())
	{
		// Skip invalid positions
		if (entity->GetPosition() == Vector3(0, 0, 0))
			continue;

		// Determine player role and get appropriate configuration
		PlayerConfig config = Configs.Survivor;
		if (entity->GetPlayerRole() == true) config = Configs.Killer;

		// Filter out invalid player names
		std::wstring playerName = entity->GetPlayerName();
		if (playerName == L"Entity" || playerName.empty() || std::all_of(playerName.begin(), playerName.end(), iswspace))
			continue;

		// Skip non-Latin names to prevent rendering issues
		if (!IsLatinString(playerName))
			continue;

		// Convert world position to screen position
		Vector2 screenpos = Camera::WorldToScreen(EngineInstance->GetCameraCache().POV, entity->GetPosition());
		if (screenpos == Vector2::Zero())
			continue;

		// Calculate distance from camera to player
		Vector3 campos = Vector3(EngineInstance->GetCameraCache().POV.Location.X, EngineInstance->GetCameraCache().POV.Location.Y, EngineInstance->GetCameraCache().POV.Location.Z);
		float distance = (Vector3::Distance(campos, entity->GetPosition()) / 39.62f) - 6;
		if (distance < 0)
			continue;

		// Format distance string based on configuration
		std::wstring wdistance = config.Distance ? L"[" + std::to_wstring((int)distance) + L"m]" : L"";
		
		// Get character name based on configuration
		std::wstring name = config.Name ? entity->GetCharacterName() : L"";

		// Skip if beyond maximum distance
		if (distance > config.MaxDistance)
			continue;

		// Render player information
		DrawText(screenpos.x, screenpos.y, name + L":" + wdistance, "Verdana", config.FontSize, config.TextColour, CentreCentre);
		DrawText(screenpos.x, screenpos.y + 80, entity->GetPlayerName(), "Verdana", config.FontSize, config.TextColour, CentreCentre);
	}
}