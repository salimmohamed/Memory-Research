#pragma once
#include "ActorEntity.h"
#include "EngineStructs.h"
// For these offsets just 7 dumper the game and open Engine_Classes
class Engine
{
private:
	uint64_t ActorClusterOffset = 0xe0; // ULevel -> ActorCluster
	uint64_t OwningGameInstance = 0x1D8; // World -> OwningGameInstance // DONE
	uint64_t PersistentLevel = 0x30; // World -> PersistentLevel // DONE
	uint64_t GWorld = 0X9DAE3C8; // 
	uint64_t LocalPlayers = 0x38; // GameInstance -> LocalPlayers // DONE
	uint64_t PlayerController = 0x30; // Player -> PlayerController // DONE
	uint64_t AcknowledgedPawn = 0x350; // PlayerController -> AcknowledgedPawn // DONE
	uint64_t CameraManager = 0x360; // PlayerController -> PlayerCameraManager // DONE
	uint64_t CameraCachePrivate = 0x0; // PlayerCameraManager -> CameraCachePrivate // DONE 
	uint64_t CameraCachePrivateOffset = 0x1410; // PlayerCameraManager -> CameraCachePrivate // DONE

	// Actor-related variables
	uint64_t ActorCluster;
	uint64_t ActorArrayPtr;
	uint32_t ActorCount;

	CameraCacheEntry CameraEntry; // ScriptStruct Engine.CameraCacheEntry //
	MinimalViewInfo CameraViewInfo; // ScriptStruct Engine.MinimalViewInfo //
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