#include "Pch.h"
#include "Engine.h"
#include "ActorEntity.h"
#include "Globals.h"
#include "vmmdll.h"

Engine::Engine()
{
	printf("\n=== Process Info ===\n");
	printf("Process Name: %s\n", ProcessName.c_str());
	uint64_t base = TargetProcess.GetBaseAddress(ProcessName);
	uint64_t size = TargetProcess.GetBaseSize(ProcessName);
	printf("Base Address: %p\n", (void*)base);
	printf("Base Size: 0x%llx\n", size);

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

	printf("\n=== Actor Array Info ===\n");
	
	// Read actor array
	uint64_t OwningActor = TargetProcess.Read<uint64_t>(PersistentLevel + 0xA0);
	int MaxPacket = TargetProcess.Read<int>(PersistentLevel + 0xA8);
	
	printf("OwningActor: %p\n", (void*)OwningActor);
	printf("MaxPacket: %d\n", MaxPacket);

	// Validate actor array
	if (!OwningActor || MaxPacket <= 0 || MaxPacket > 1000) {
		printf("Invalid actor array data\n");
		return;
	}

	// Read actor array
	std::vector<uint64_t> actors(MaxPacket);
	TargetProcess.Read(OwningActor, actors.data(), MaxPacket * sizeof(uint64_t));

	// Filter out non-player actors
	std::vector<uint64_t> playerActors;
	for (int i = 0; i < MaxPacket; i++) {
		if (!actors[i]) {
			printf("Skipping null actor at index %d\n", i);
			continue;
		}

		// Try multiple offsets for actor class
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

		// Simple filtering - just check if it's not a static object
		if (className.find("Static") == std::string::npos && 
			className.find("Pickup") == std::string::npos &&
			className.find("Trap") == std::string::npos &&
			className.find("Entity") == std::string::npos &&
			className.find("Unknown") == std::string::npos) {
			playerActors.push_back(actors[i]);
		}
	}

	printf("Found %zu potential player actors\n", playerActors.size());

	// Process only player actors
	std::vector<std::shared_ptr<ActorEntity>> actorEntities;
	auto handle = TargetProcess.CreateScatterHandle();
	for (const auto& actor : playerActors) {
		std::shared_ptr<ActorEntity> entity = std::make_shared<ActorEntity>(actor, handle);
		actorEntities.push_back(entity);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

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

	// Print detailed information about each valid player
	printf("\n=== Valid Players Found ===\n");
	for (const auto& actor : Actors) {
		// Skip if player name is empty, contains "Character:", or is just "Character:"
		if (actor->GetPlayerName().empty() || 
			actor->GetPlayerName().find(L"Character:") != std::wstring::npos ||
			actor->GetPlayerName() == L"Character:") {
			continue;
		}

		// Skip if character name is empty, "Entity", "Unknown", or contains "Character:"
		if (actor->GetCharacterName().empty() || 
			actor->GetCharacterName() == L"Entity" || 
			actor->GetCharacterName() == L"Unknown" ||
			actor->GetCharacterName().find(L"Character:") != std::wstring::npos) {
			continue;
		}

		// Skip if position is zero (invalid position)
		if (actor->GetPosition().x == 0.0f && 
			actor->GetPosition().y == 0.0f && 
			actor->GetPosition().z == 0.0f) {
			continue;
		}

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


void Engine::Cache()
{
	// Cache function is now empty since we moved everything to constructor
}

void Engine::UpdatePlayers()
{
	auto handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<ActorEntity> entity : Actors)
	{
		entity->UpdatePosition(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
}

void Engine::RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, CameraManager + CameraCachePrivate, &CameraEntry, sizeof(CameraCacheEntry));
}

CameraCacheEntry Engine::GetCameraCache()
{
	return CameraEntry;
}

std::vector<std::shared_ptr<ActorEntity>> Engine::GetActors()
{
	return Actors;
}

uint32_t Engine::GetActorSize()
{
	return MaxPacket;
}
