#include "Pch.h"
#include "Engine.h"
#include "ActorEntity.h"
#include "Globals.h"
#include "vmmdll.h"

/**
 * Engine Constructor
 * 
 * Initializes the Engine class by:
 * 1. Reading process information
 * 2. Setting up object hierarchy
 * 3. Reading and filtering actors
 * 4. Creating ActorEntity objects
 * 5. Printing detailed player information
 */
Engine::Engine()
{
	// Print process information
	printf("\n=== Process Info ===\n");
	printf("Process Name: %s\n", ProcessName.c_str());
	uint64_t base = TargetProcess.GetBaseAddress(ProcessName);
	uint64_t size = TargetProcess.GetBaseSize(ProcessName);
	printf("Base Address: %p\n", (void*)base);
	printf("Base Size: 0x%llx\n", size);

	// Set up object hierarchy by reading from game memory
	printf("\n=== Object Hierarchy ===\n");
	GWorld = TargetProcess.Read<uint64_t>(base + GWorld);
	printf("GWorld: %p\n", GWorld);
	PersistentLevel = TargetProcess.Read<uint64_t>(GWorld + PersistentLevel);
	printf("PersistentLevel: %p\n", PersistentLevel);
	OwningGameInstance = TargetProcess.Read<uint64_t>(GWorld + OwningGameInstance);
	printf("OwningGameInstance: %p\n", OwningGameInstance);
	LocalPlayers = TargetProcess.Read<uint64_t>(OwningGameInstance + LocalPlayers);
	printf("LocalPlayers: %p\n", LocalPlayers);
	LocalPlayers = TargetProcess.Read<uint64_t>(LocalPlayers);
	printf("LocalPlayers dereferenced: %p\n", LocalPlayers);
	PlayerController = TargetProcess.Read<uint64_t>(LocalPlayers + PlayerController);
	printf("PlayerController: %p\n", PlayerController);
	AcknowledgedPawn = TargetProcess.Read<uint64_t>(PlayerController + AcknowledgedPawn);
	printf("AcknowledgedPawn: %p\n", AcknowledgedPawn);
	CameraManager = TargetProcess.Read<uint64_t>(PlayerController + CameraManager);
	printf("CameraManager: %p\n", CameraManager);
	CameraEntry = TargetProcess.Read<CameraCacheEntry>(CameraManager + CameraCachePrivate);
	printf("CameraCacheEntry: %p\n", CameraEntry);

	// Read and process actor array
	printf("\n=== Actor Array Info ===\n");
	
	// Read actor array information
	uint64_t OwningActor = TargetProcess.Read<uint64_t>(PersistentLevel + 0xA0);
	int MaxPacket = TargetProcess.Read<int>(PersistentLevel + 0xA8);
	
	printf("OwningActor: %p\n", (void*)OwningActor);
	printf("MaxPacket: %d\n", MaxPacket);

	// Validate actor array data
	if (!OwningActor || MaxPacket <= 0 || MaxPacket > 1000) {
		printf("Invalid actor array data\n");
		return;
	}

	// Read all actors from memory
	std::vector<uint64_t> actors(MaxPacket);
	TargetProcess.Read(OwningActor, actors.data(), MaxPacket * sizeof(uint64_t));

	// Filter actors to find potential players
	std::vector<uint64_t> playerActors;
	for (int i = 0; i < MaxPacket; i++) {
		if (!actors[i]) {
			printf("Skipping null actor at index %d\n", i);
			continue;
		}

		// Try multiple offsets to find actor class
		std::vector<uint64_t> possibleClassOffsets = {0x10, 0x18, 0x20, 0x28, 0x30};
		bool foundValidClass = false;
		std::string className;

		for (uint64_t offset : possibleClassOffsets) {
			uint64_t actorClass = TargetProcess.Read<uint64_t>(actors[i] + offset);
			if (!actorClass) continue;

			uint64_t namePtr = TargetProcess.Read<uint64_t>(actorClass + 0x10);
			if (!namePtr) continue;

			char nameBuffer[64] = {0};
			TargetProcess.Read(namePtr, nameBuffer, sizeof(nameBuffer));
			std::string name(nameBuffer);

			if (!name.empty() && name != "None") {
				className = name;
				foundValidClass = true;
				break;
			}
		}

		if (!foundValidClass) {
			continue;
		}

		// Filter out non-player actors based on class name
		if (className.find("Static") == std::string::npos && 
			className.find("Pickup") == std::string::npos &&
			className.find("Trap") == std::string::npos &&
			className.find("Entity") == std::string::npos &&
			className.find("Unknown") == std::string::npos) {
			playerActors.push_back(actors[i]);
		}
	}

	printf("Found %zu potential player actors\n", playerActors.size());

	// Create ActorEntity objects for each player actor
	std::vector<std::shared_ptr<ActorEntity>> actorEntities;
	auto handle = TargetProcess.CreateScatterHandle();
	for (const auto& actor : playerActors) {
		std::shared_ptr<ActorEntity> entity = std::make_shared<ActorEntity>(actor, handle);
		actorEntities.push_back(entity);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	// Set up actor entities in two phases
	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<ActorEntity> entity : actorEntities) {
		entity->SetUp1(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	for (std::shared_ptr<ActorEntity> entity : actorEntities) {
		entity->SetUp2();
	}

	Actors = actorEntities;
	printf("Found %zu valid players\n", Actors.size());

	// Print detailed information about valid players
	printf("\n=== Valid Players Found ===\n");
	for (const auto& actor : Actors) {
		// Skip invalid player names
		if (actor->GetPlayerName().empty() || 
			actor->GetPlayerName().find(L"Character:") != std::wstring::npos ||
			actor->GetPlayerName() == L"Character:") {
			continue;
		}

		// Skip invalid character names
		if (actor->GetCharacterName().empty() || 
			actor->GetCharacterName() == L"Entity" || 
			actor->GetCharacterName() == L"Unknown" ||
			actor->GetCharacterName().find(L"Character:") != std::wstring::npos) {
			continue;
		}

		// Skip invalid positions
		if (actor->GetPosition().x == 0.0f && 
			actor->GetPosition().y == 0.0f && 
			actor->GetPosition().z == 0.0f) {
			continue;
		}

		// Print player information
		printf("Player: %ls\n", actor->GetPlayerName().c_str());
		printf("Character: %ls\n", actor->GetCharacterName().c_str());
		printf("Role: %s\n", actor->GetPlayerRole() ? "Terror" : "Human");
		printf("Position: %.2f, %.2f, %.2f\n", 
			actor->GetPosition().x, 
			actor->GetPosition().y, 
			actor->GetPosition().z);
		printf("-------------------\n");
	}
}

/**
 * Cache function
 * Currently empty as initialization is handled in constructor
 */
void Engine::Cache()
{
	// Cache function is now empty since we moved everything to constructor
}

/**
 * Checks if a new game has started
 * Compares current actor count with last known count
 * @return true if significant change in actor count detected
 */
bool Engine::CheckForNewGame()
{
	uint32_t currentActorCount = GetActorSize();
	
	// Initialize last count if not set
	if (LastActorCount == 0)
	{
		LastActorCount = currentActorCount;
		return false;
	}

	// Detect significant changes in actor count
	if (abs((int)currentActorCount - (int)LastActorCount) > (LastActorCount * 0.1))
	{
		LastActorCount = currentActorCount;
		return true;
	}

	return false;
}

/**
 * Reloads all actors
 * Clears existing actors and re-caches them
 */
void Engine::ReloadActors()
{
	// Clear existing actors
	Actors.clear();
	
	// Re-cache the actors
	Cache();
}

/**
 * Updates player information
 * Checks for new game and updates positions
 */
void Engine::UpdatePlayers()
{
	// Check if we need to reload actors
	if (CheckForNewGame())
	{
		ReloadActors();
	}

	// Update positions for all actors
	auto handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<ActorEntity> entity : Actors)
	{
		entity->UpdatePosition(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
}

/**
 * Refreshes the view matrix
 * Updates camera cache entry from game memory
 * @param handle - Scatter handle for memory operations
 */
void Engine::RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, CameraManager + CameraCachePrivate, &CameraEntry, sizeof(CameraCacheEntry));
}

/**
 * Gets the current camera cache entry
 * @return Current CameraCacheEntry structure
 */
CameraCacheEntry Engine::GetCameraCache()
{
	return CameraEntry;
}

/**
 * Gets the list of tracked actors
 * @return Vector of shared pointers to ActorEntity objects
 */
std::vector<std::shared_ptr<ActorEntity>> Engine::GetActors()
{
	return Actors;
}

/**
 * Gets the current number of actors
 * @return Number of tracked actors
 */
uint32_t Engine::GetActorSize()
{
	return MaxPacket;
}
