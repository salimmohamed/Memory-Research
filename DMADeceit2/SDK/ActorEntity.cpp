#include "Pch.h"
#include "ActorEntity.h"
#include "Camera.h"
#include "Globals.h"
#include <string>
#include <set>

ActorEntity::ActorEntity(uint64_t address,VMMDLL_SCATTER_HANDLE handle)
{
	Class = address;
	if(!address)
		return;  //Class = AActor
	 TargetProcess.AddScatterReadRequest(handle, Class + PlayerState, reinterpret_cast<void*>(&PlayerState), sizeof(uint64_t));
	 TargetProcess.AddScatterReadRequest(handle,Class + AcknowledgedPawn, reinterpret_cast<void*>(&AcknowledgedPawn),sizeof(uint64_t));
	 TargetProcess.AddScatterReadRequest(handle, Class + RootComponent, reinterpret_cast<void*>(&RootComponent), sizeof(uint64_t));

	
}

void ActorEntity::SetUp1(VMMDLL_SCATTER_HANDLE handle)
{
	if (!Class)
		return;
	if (!RootComponent)
		return;
	//if (!AcknowledgedPawn && PlayerState) // players aren't pawns
	//if (PlayerState) // players aren't pawns
	//{
	TargetProcess.AddScatterReadRequest(handle, PlayerState + GameRole, reinterpret_cast<void*>(&PlayerRole), sizeof(EPlayerRole));
	TargetProcess.AddScatterReadRequest(handle, PlayerState + GameCharacterName, reinterpret_cast<void*>(&CharacterNameId), sizeof(EDeceitCharacter));
	TargetProcess.AddScatterReadRequest(handle, PlayerState + GamePlayerNameOffset, reinterpret_cast<void*>(&PlayerNameAddress), sizeof(64));
	//}
}


std::wstring ReadStringFromMemory(uint64_t address) {
	std::wstring result;
	wchar_t character;

	// Loop to read each character until the null terminator is found
	while (true) {
		character = TargetProcess.Read<wchar_t>(address);
		if (character == L'\0') {
			break;
		}
		result += character;
		address += sizeof(wchar_t); // Move to the next character
	}

	return result;
}

// Static set to track used character names
static std::set<std::wstring> usedCharacterNames;

void ActorEntity::SetUp2()
{

	if (!Class)
		return;
	if (!RootComponent)
		return;
	//if (!AcknowledgedPawn && PlayerState) // players aren't pawns
	if (AcknowledgedPawn && PlayerState) // players aren't pawns
	{
		// Skip if AcknowledgedPawn is invalid (too small or too large)
		if (AcknowledgedPawn < 0x10000 || AcknowledgedPawn > 0x7FFFFFFFFFFFFFFF)
			return;

		if (PlayerRole == EPlayerRole::EDeceitRole__Invalid)
			return;

		CharacterName = GetPlayerNameById(CharacterNameId);
		uint64_t test = TargetProcess.Read<uint64_t>(PlayerState + GamePlayerNameOffset);
		TerrorCosmetic = TargetProcess.Read<int>(PlayerState + TerrorCosmeticOffset);
		std::wstring playerName = ReadStringFromMemory(test);
		PlayerName = playerName;

		// Skip if position is zero or invalid (static/fake entity)
		UEPosition = TargetProcess.Read<UEVector>(RootComponent + RelativeLocation);
		Position = Vector3(UEPosition.X, UEPosition.Y, UEPosition.Z);
		
		// Check for invalid positions (zero, infinity, or NaN)
		if (Position.x == 0.0f && Position.y == 0.0f && Position.z == 0.0f)
			return;
		if (!std::isfinite(Position.x) || !std::isfinite(Position.y) || !std::isfinite(Position.z))
			return;
		// Skip if position is too small (likely invalid)
		if (std::abs(Position.x) < 0.1f && std::abs(Position.y) < 0.1f && std::abs(Position.z) < 0.1f)
			return;

		// Skip if character name is a single letter (likely invalid)
		if (CharacterName.length() == 1)
			return;

		// Only allow exact game character names
		static const std::vector<std::wstring> validNames = {
			L"Alex", L"Lisa", L"Chang", L"Rachel", L"Hans", L"Nina", L"Jak", L"Priya", L"Beck"
		};
		bool isValidName = false;
		for (const auto& validName : validNames) {
			if (playerName == validName) {
				// Check if this character is already in use
				if (usedCharacterNames.find(validName) != usedCharacterNames.end())
					return; // Skip if character is already taken
				usedCharacterNames.insert(validName);
				isValidName = true;
				break;
			}
		}
		if (!isValidName)
			return;

		// Skip if CharacterNameId is invalid (should match one of the valid characters)
		if (static_cast<int>(CharacterNameId) == 0 || static_cast<int>(CharacterNameId) > 10) // Assuming IDs are 1-9 for the 9 characters
			return;

		// Print debug info for all valid players (including cursed)
		printf("\n=== New Player Found ===\n");
		printf("PlayerState: %p\n", PlayerState);
		printf("AcknowledgedPawn: %p\n", AcknowledgedPawn);
		printf("RootComponent: %p\n", RootComponent);
		printf("CharacterNameId: %d\n", CharacterNameId);
		printf("TerrorCosmetic: %d\n", TerrorCosmetic);
		printf("PlayerRole: %d\n", PlayerRole);
		printf("Position: %.2f, %.2f, %.2f\n", Position.x, Position.y, Position.z);
		printf("CharacterName: %s\n", CharacterName.c_str());
		printf("PlayerName: %s\n", PlayerName.c_str());
		
		// Check for cursed roles
		if (IsCursed()) {
			printf("*** CURSED PLAYER DETECTED: %s ***\n", GetCursedRoleName().c_str());
		}
	}

}

bool ActorEntity::GetPlayerRole()
{
	return TerrorCosmetic;
}

EPlayerRole ActorEntity::GetSpecificPlayerRole()
{
	return PlayerRole;
}

bool ActorEntity::IsCursed()
{
	// Check if player has a cursed role (Mimic, Soulbound, Phantom, Chemist)
	return (PlayerRole == EPlayerRole::EDeceitRole__Mimic ||
			PlayerRole == EPlayerRole::EDeceitRole__Soulbound ||
			PlayerRole == EPlayerRole::EDeceitRole__Phantom ||
			PlayerRole == EPlayerRole::EDeceitRole__Chemist);
}

std::wstring ActorEntity::GetCursedRoleName()
{
	switch (PlayerRole) {
	case EPlayerRole::EDeceitRole__Mimic:
		return L"Mimic";
	case EPlayerRole::EDeceitRole__Soulbound:
		return L"Soulbound";
	case EPlayerRole::EDeceitRole__Phantom:
		return L"Phantom";
	case EPlayerRole::EDeceitRole__Chemist:
		return L"Chemist";
	default:
		return L"";
	}
}

EDeceitCharacter ActorEntity::GetCharacterNameId()
{
	return CharacterNameId;
}

uint64_t ActorEntity::GetClass()
{
	return Class;
}

std::wstring ActorEntity::GetCharacterName()
{
	return CharacterName;
}

std::wstring ActorEntity::GetPlayerName()
{
	return PlayerName;
}

Vector3 ActorEntity::GetPosition()
{
	Position = Vector3(UEPosition.X, UEPosition.Y, UEPosition.Z);
	return Position;
}

void ActorEntity::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	if (!Class)
		return;
	if (!RootComponent)
		return;
	if (!PlayerState)
		return;
	if (!AcknowledgedPawn) // players aren't pawns
		return;
	/*if (PlayerRole != EPlayerRole::EDeceitRole__Human && PlayerRole != EPlayerRole::EDeceitRole__Terror)
		return;*/
	/*if (PlayerRole != EPlayerRole::EDeceitRole__Invalid)
		return;*/
	TargetProcess.AddScatterReadRequest(handle, RootComponent + RelativeLocation, reinterpret_cast<void*>(&UEPosition), sizeof(UEVector));
}