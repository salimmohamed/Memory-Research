#include "Pch.h"
#include "Engine.h"
#include "ActorEntity.h"
#include "Globals.h"

Engine::Engine()
{
    GWorld = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(ProcessName) + GWorld);
    printf("GWorld: %p\n", (void*)GWorld);
    PersistentLevel = TargetProcess.Read<uint64_t>(GWorld + PersistentLevel);
    printf("PersistentLevel: %p\n", (void*)PersistentLevel);
    OwningGameInstance = TargetProcess.Read<uint64_t>(GWorld + OwningGameInstance);
    printf("OwningGameInstance: %p\n", (void*)OwningGameInstance);
    LocalPlayers = TargetProcess.Read<uint64_t>(OwningGameInstance + LocalPlayers);
    printf("LocalPlayers: %p\n", (void*)LocalPlayers);
    LocalPlayers = TargetProcess.Read<uint64_t>(LocalPlayers);
    printf("LocalPlayers: %p\n", (void*)LocalPlayers);
    PlayerController = TargetProcess.Read<uint64_t>(LocalPlayers + PlayerController);
    printf("PlayerController: %p\n", (void*)PlayerController);
    AcknowledgedPawn = TargetProcess.Read<uint64_t>(PlayerController + AcknowledgedPawn);
    printf("AcknowledgedPawn: %p\n", (void*)AcknowledgedPawn);
    CameraManager = TargetProcess.Read<uint64_t>(PlayerController + CameraManager);
    printf("CameraManager: %p\n", (void*)CameraManager);
    CameraEntry = TargetProcess.Read<CameraCacheEntry>(CameraManager + CameraCachePrivateOffset);
    printf("CameraCacheEntry: %p\n", (void*)&CameraEntry);
}

void Engine::Cache()
{
    // Get actor array using the confirmed working offset
    ActorArrayPtr = TargetProcess.Read<uint64_t>(PersistentLevel + ActorsOffset);
    printf("Level Actors (offset 0xa0): %p\n", (void*)ActorArrayPtr);

    // Get actor count
    ActorCount = TargetProcess.Read<uint32_t>(PersistentLevel + ActorCountOffset);
    printf("Level Actor Count: %d\n", ActorCount);

    // Sanity check to avoid processing invalid data
    if (!ActorArrayPtr) {
        printf("Invalid actor array\n");
        return;
    }

    if (ActorCount <= 0 || ActorCount > 10000) {
        printf("Invalid actor count, using default\n");
        ActorCount = 2000; // Reasonable default
    }

    printf("Using ULevel direct actor array\n");
    printf("Final Actor Array: %p\n", (void*)ActorArrayPtr);
    printf("Actor Count: %d\n", ActorCount);

    std::vector<uint64_t> entitylist;
    entitylist.resize(ActorCount);
    std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(ActorCount);
    TargetProcess.Read(ActorArrayPtr, object_raw_ptr.get(), ActorCount * sizeof(uint64_t));

    for (size_t i = 0; i < ActorCount; i++) {
        entitylist[i] = object_raw_ptr[i];
    }

    std::list<std::shared_ptr<ActorEntity>> actors;
    auto handle = TargetProcess.CreateScatterHandle();

    for (uint64_t address : entitylist) {
        uintptr_t actor = address;
        if (!actor)
            continue;

        std::shared_ptr<ActorEntity> entity = std::make_shared<ActorEntity>(actor, handle);
        actors.push_back(entity);
    }

    printf("Total actors initially: %d\n", (int)actors.size());

    TargetProcess.ExecuteReadScatter(handle);
    TargetProcess.CloseScatterHandle(handle);

    handle = TargetProcess.CreateScatterHandle();
    for (std::shared_ptr<ActorEntity> entity : actors) {
        entity->SetUp1(handle);
    }

    TargetProcess.ExecuteReadScatter(handle);
    TargetProcess.CloseScatterHandle(handle);

    std::vector<std::shared_ptr<ActorEntity>> playerlist;
    int validCount = 0;

    for (std::shared_ptr<ActorEntity> entity : actors) {
        entity->SetUp2();

        // Check for non-zero position and role != 0
        Vector3 pos = entity->GetPosition();
        bool isRoleValid = entity->GetPlayerRole();

        if (pos != Vector3::Zero() && (pos.x == pos.x) && (pos.y == pos.y) && (pos.z == pos.z)) {  // Filter out NaN positions
            if (isRoleValid) {  // Only include entities with a valid role
                playerlist.push_back(entity);

                // Print debug info for the first 5 valid entities
                if (validCount < 5) {
                    printf("Valid Entity[%d]: Pos(%f, %f, %f) Role=%d Name=%ls\n",
                        validCount, pos.x, pos.y, pos.z,
                        isRoleValid, entity->GetPlayerName().c_str());
                }
                validCount++;
            }
        }
    }

    printf("Valid player entities with role != 0 and valid position: %d\n", validCount);

    if (playerlist.empty()) {
        printf("No players matching criteria, adding entities with any valid position...\n");
        playerlist.clear();

        for (std::shared_ptr<ActorEntity> entity : actors) {
            Vector3 pos = entity->GetPosition();
            if (pos != Vector3::Zero() && (pos.x == pos.x) && (pos.y == pos.y) && (pos.z == pos.z)) {
                playerlist.push_back(entity);
                if (playerlist.size() <= 5) {
                    printf("Fallback Entity[%d]: Pos(%f, %f, %f) Role=%d Name=%ls\n",
                        (int)playerlist.size() - 1, pos.x, pos.y, pos.z,
                        entity->GetPlayerRole(), entity->GetPlayerName().c_str());
                }
            }
        }
    }

    Actors = playerlist;
    printf("Final player count: %d\n", (int)Actors.size());
}

void Engine::UpdatePlayers()
{
    auto handle = TargetProcess.CreateScatterHandle();
    for (std::shared_ptr<ActorEntity> entity : Actors) {
        entity->UpdatePosition(handle);
    }
    TargetProcess.ExecuteReadScatter(handle);
    TargetProcess.CloseScatterHandle(handle);
}

void Engine::RefreshViewMatrix(VMMDLL_SCATTER_HANDLE handle)
{
    TargetProcess.AddScatterReadRequest(handle, CameraManager + CameraCachePrivateOffset, reinterpret_cast<void*>(&CameraEntry), sizeof(CameraCacheEntry));
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
    return ActorCount;
}