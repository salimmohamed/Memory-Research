#pragma once
#include "ActorEntity.h"
#include "EngineStructs.h"

/**
 * Engine Class
 * 
 * This class serves as the core interface to the Unreal Engine game state.
 * It provides functionality for:
 * - Managing game world state
 * - Handling player and actor updates
 * - Camera management
 * - Game instance access
 * 
 * The class uses memory offsets to access various Unreal Engine structures
 * and maintain synchronization with the game state.
 */

class Engine
{
private:
	// Memory offsets for accessing Unreal Engine structures
	uint64_t OwningActorOffset = 0x98;    // UNetConnection -> OwningActor
	uint64_t MaxPacketOffset = 0xa0;      // UNetConnection -> MaxPacket
	uint64_t OwningActor;                 // Current owning actor
	uint64_t MaxPacket;                   // Maximum packet size
	uint64_t OwningGameInstance = 0x1d8;  // World -> OwningGameInstance
	uint64_t PersistentLevel = 0x30;      // World -> PersistentLevel
	uint64_t GWorld = 0x9DBE5C8;          // GWorld offset from base
	uint64_t LocalPlayers = 0x38;         // GameInstance -> LocalPlayers
	uint64_t PlayerController = 0x30;     // Player -> PlayerController
	uint64_t AcknowledgedPawn = 0x350;    // PlayerController -> AcknowledgedPawn
	uint64_t CameraManager = 0x360;       // PlayerController -> PlayerCameraManager
	uint64_t CameraCachePrivate = 0x1410; // PlayerCameraManager -> CameraCachePrivate

	// State tracking
	uint32_t LastActorCount = 0;          // Last known actor count
	bool bShouldReload = false;           // Flag indicating if actors need reloading

	// Camera and view data
	CameraCacheEntry CameraEntry;         // Current camera cache entry
	MinimalViewInfo CameraViewInfo;       // Current camera view information
	std::vector<std::shared_ptr<ActorEntity>> Actors; // List of tracked actors

public:
	/**
	 * Constructor
	 * Initializes the Engine class and its components
	 */
	Engine();

	/**
	 * Caches current game state
	 * Updates internal state with current game data
	 */
	void Cache();

	/**
	 * Updates player information
	 * Refreshes data for all tracked players
	 */
	void UpdatePlayers();

	/**
	 * Gets the list of tracked actors
	 * @return Vector of shared pointers to ActorEntity objects
	 */
	std::vector<std::shared_ptr<ActorEntity>> GetActors();

	/**
	 * Gets the current camera cache entry
	 * @return Current CameraCacheEntry structure
	 */
	CameraCacheEntry GetCameraCache();

	/**
	 * Refreshes the view matrix
	 * Updates camera view information from game memory
	 * @param handle - Scatter handle for memory operations
	 */
	void RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle);

	/**
	 * Gets the current number of actors
	 * @return Number of tracked actors
	 */
	uint32_t GetActorSize();

	/**
	 * Checks if a new game has started
	 * @return true if a new game has started, false otherwise
	 */
	bool CheckForNewGame();

	/**
	 * Reloads all actors
	 * Forces a refresh of the actor list
	 */
	void ReloadActors();
};