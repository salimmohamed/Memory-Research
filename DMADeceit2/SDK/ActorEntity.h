#pragma once
#include "EngineStructs.h"
#include <iostream>
#include <string>

/**
 * Actor Entity System
 * 
 * This system handles all game entities (players, objects, etc.) in the game world.
 * It provides functionality for:
 * - Reading entity data from game memory
 * - Managing entity states and properties
 * - Handling player roles and characters
 * - Processing entity positions and names
 */

/**
 * Player role enumeration
 * Defines the different roles a player can have in the game
 */
enum class EPlayerRole : uint8_t 
{ 
	EDeceitRole__Human = 0,      // Regular human player
	EDeceitRole__Terror = 1,     // Terror player
	EDeceitRole__Medic = 2,      // Medic role
	EDeceitRole__Warden = 3,     // Warden role
	EDeceitRole__Inquisitor = 4, // Inquisitor role
	EDeceitRole__Mimic = 5,      // Mimic role
	EDeceitRole__Vector = 6,     // Vector role
	EDeceitRole__Seer = 7,       // Seer role
	EDeceitRole__Purifier = 8,   // Purifier role
	EDeceitRole__Medium = 9,     // Medium role
	EDeceitRole__Chemist = 10,   // Chemist role
	EDeceitRole__Soulbound = 11, // Soulbound role
	EDeceitRole__Phantom = 12,   // Phantom role
	EDeceitRole__Invalid = 13    // Invalid/unknown role
};

/**
 * Character enumeration
 * Defines the different playable characters in the game
 */
enum class EDeceitCharacter : uint8_t {
	Alex = 0,    // Alex character
	Lisa = 1,    // Lisa character
	Chang = 2,   // Chang character
	Rachel = 3,  // Rachel character
	Hans = 4,    // Hans character
	Nina = 5,    // Nina character
	Jak = 6,     // Jak character
	Priya = 7,   // Priya character
	Beck = 8,    // Beck character
	Last = 9     // Last character
};

/**
 * Terror type enumeration
 * Defines the different types of terror characters
 */
enum class EDeceitTerror : uint8_t {
	EDeceitTerror__Experiment = 0, // Experiment terror
	EDeceitTerror__Werewolf = 1,   // Werewolf terror
	EDeceitTerror__Vampire = 2,    // Vampire terror
	EDeceitTerror__Clown = 3,      // Clown terror
	EDeceitTerror__Last = 4        // Last terror type
};

/**
 * Converts character enum to display name
 * @param character - Character enum value
 * @return Display name as wide string
 */
inline std::wstring GetPlayerNameById(EDeceitCharacter character) {
	switch (character) {
	case EDeceitCharacter::Alex: return L"Alex";
	case EDeceitCharacter::Lisa: return L"Lisa";
	case EDeceitCharacter::Chang: return L"Chang";
	case EDeceitCharacter::Rachel: return L"Rachel";
	case EDeceitCharacter::Hans: return L"Hans";
	case EDeceitCharacter::Nina: return L"Nina";
	case EDeceitCharacter::Jak: return L"Jak";
	case EDeceitCharacter::Priya: return L"Priya";
	case EDeceitCharacter::Beck: return L"Beck";
	case EDeceitCharacter::Last: return L"Last";
	default: return L"Unknown";
	}
}

/**
 * Actor Entity Class
 * 
 * This class represents a game entity in the world.
 * It handles:
 * - Reading entity data from memory
 * - Managing entity properties
 * - Updating entity states
 * - Providing access to entity information
 */
class ActorEntity
{
private:
	// Memory offsets for entity data
	uint64_t Class = 0;
	EPlayerRole PlayerRole;
	bool TerrorCosmetic;
	EDeceitCharacter CharacterNameId;
	uint64_t PlayerNameAddress;
	uint64_t PlayerState = 0x2c8;        // Pawn -> PlayerState
	uint64_t AcknowledgedPawn = 0x350;   // Pawn -> AcknowledgedPawn
	uint64_t RootComponent = 0x1b8;      // Actor -> RootComponent
	uint64_t RelativeLocation = 0x128;   // SceneComponent -> RelativeLocation
	uint64_t GameRole = 0x688;           // ADeceitPlayerState -> ChosenRole
	uint64_t TerrorCosmeticOffset = 0x548; // is infected
	uint64_t GameCharacterName = 0x528;  // ADeceitPlayerState -> Character
	uint64_t GamePlayerNameOffset = 0x340; // APlayerState -> PlayerNamePrivate

	// Entity properties
	std::wstring CharacterName = LIT(L"Entity");
	std::wstring PlayerName = LIT(L"Entity");
	UEVector UEPosition;
	Vector3 Position;

public:
	/**
	 * Constructor
	 * @param address - Memory address of the entity
	 * @param handle - Scatter handle for memory operations
	 */
	ActorEntity(uint64_t address, VMMDLL_SCATTER_HANDLE handle);

	/**
	 * Initial setup of entity data
	 * @param handle - Scatter handle for memory operations
	 */
	void SetUp1(VMMDLL_SCATTER_HANDLE handle);

	/**
	 * Secondary setup of entity data
	 */
	void SetUp2();

	/**
	 * Gets the entity's class address
	 * @return Class address
	 */
	uint64_t GetClass();

	/**
	 * Gets the player's role
	 * @return true if player is a terror, false otherwise
	 */
	bool GetPlayerRole();

	/**
	 * Gets the specific player role enum
	 * @return Player role enum value
	 */
	EPlayerRole GetSpecificPlayerRole();

	/**
	 * Checks if player is cursed (Mimic, Soulbound, Phantom, Chemist)
	 * @return true if player is cursed, false otherwise
	 */
	bool IsCursed();

	/**
	 * Gets the cursed role name as string
	 * @return Cursed role name or empty string if not cursed
	 */
	std::wstring GetCursedRoleName();

	/**
	 * Gets the character ID
	 * @return Character enum value
	 */
	EDeceitCharacter GetCharacterNameId();

	/**
	 * Gets the character's display name
	 * @return Character name as wide string
	 */
	std::wstring GetCharacterName();

	/**
	 * Gets the player's name
	 * @return Player name as wide string
	 */
	std::wstring GetPlayerName();

	/**
	 * Gets the entity's position
	 * @return Position as Vector3
	 */
	Vector3 GetPosition();

	/**
	 * Updates the entity's position
	 * @param handle - Scatter handle for memory operations
	 */
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
};