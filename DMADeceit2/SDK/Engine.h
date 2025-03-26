#pragma once
#include "ActorEntity.h"
#include "EngineStructs.h"
// For these offsets just 7 dumper the game and open Engine_Classes
class Engine
{
private:
	uint64_t ActorsOffset = 0xA0;      // ULevel -> Actors array (confirmed working)
	uint64_t ActorCountOffset = 0xA8;  // ULevel -> Actor count (typical offset after array)
	uint64_t OwningGameInstance = 0x1D8; // World -> OwningGameInstance
	uint64_t PersistentLevel = 0x30; // World -> PersistentLevel
	uint64_t GWorld = 0X9DAE3C8; // GWorld offset from base
	uint64_t LocalPlayers = 0x38; // GameInstance -> LocalPlayers
	uint64_t PlayerController = 0x30; // Player -> PlayerController
	uint64_t AcknowledgedPawn = 0x350; // PlayerController -> AcknowledgedPawn
	uint64_t CameraManager = 0x360; // PlayerController -> PlayerCameraManager
	uint64_t CameraCachePrivateOffset = 0x1410; // PlayerCameraManager -> CameraCachePrivate

	// Actor array information
	uint64_t ActorArrayPtr;
	uint32_t ActorCount;

	CameraCacheEntry CameraEntry; // ScriptStruct Engine.CameraCacheEntry
	MinimalViewInfo CameraViewInfo; // ScriptStruct Engine.MinimalViewInfo
	std::vector<std::shared_ptr<ActorEntity>> Actors;
public:
	Engine();
	void Cache();
	void UpdatePlayers();
	std::vector<std::shared_ptr<ActorEntity>> GetActors();
	CameraCacheEntry GetCameraCache();
	void RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle);
	uint32_t GetActorSize();
};