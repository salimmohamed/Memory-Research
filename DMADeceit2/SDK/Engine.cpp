#include "Pch.h"
#include "Engine.h"
#include "ActorEntity.h"
#include "Globals.h"

Engine::Engine()
{
    GWorld = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress(ProcessName) + GWorld);
    printf("GWorld: %p\n", GWorld);
    PersistentLevel = TargetProcess.Read<uint64_t>(GWorld + PersistentLevel);
    printf("PersistentLevel: %p\n", PersistentLevel);
    OwningGameInstance = TargetProcess.Read<uint64_t>(GWorld + OwningGameInstance);
    printf("OwningGameInstance: %p\n", OwningGameInstance);
    LocalPlayers = TargetProcess.Read<uint64_t>(OwningGameInstance + LocalPlayers);
    printf("LocalPlayers: %p\n", LocalPlayers);
    LocalPlayers = TargetProcess.Read<uint64_t>(LocalPlayers);
    printf("LocalPlayers: %p\n", LocalPlayers);
    PlayerController = TargetProcess.Read<uint64_t>(LocalPlayers + PlayerController);
    printf("PlayerController: %p\n", PlayerController);
    AcknowledgedPawn = TargetProcess.Read<uint64_t>(PlayerController + AcknowledgedPawn);
    printf("AcknowledgedPawn: %p\n", AcknowledgedPawn);
    CameraManager = TargetProcess.Read<uint64_t>(PlayerController + CameraManager);
    printf("CameraManager: %p\n", CameraManager);
    CameraEntry = TargetProcess.Read<CameraCacheEntry>(CameraManager + CameraCachePrivateOffset);
    printf("CameraCacheEntry: %p\n", CameraEntry);
}

void Engine::Cache()
{
    bool foundValidActors = false;

    // Get the ActorCluster
    ActorCluster = TargetProcess.Read<uint64_t>(PersistentLevel + ActorClusterOffset);
    printf("Actor Cluster: %p\n", ActorCluster);

    if (!ActorCluster) {
        printf("Invalid actor cluster\n");

        // Try using Children from AActor (offset 0x1a8)
        uint64_t worldActorChildren = TargetProcess.Read<uint64_t>(GWorld + 0x1a8);
        printf("World Actor Children: %p\n", worldActorChildren);

        if (worldActorChildren) {
            uint64_t childrenArrayPtr = TargetProcess.Read<uint64_t>(worldActorChildren);
            uint32_t childrenCount = TargetProcess.Read<uint32_t>(worldActorChildren + 0x8);
            printf("Children Array Ptr: %p\n", childrenArrayPtr);
            printf("Children Count: %d\n", childrenCount);

            if (childrenArrayPtr && childrenCount > 0 && childrenCount < 10000) {
                ActorArrayPtr = childrenArrayPtr;
                ActorCount = childrenCount;
                printf("Using World Children as Actor Array\n");
                foundValidActors = true;
            }
        }

        if (!foundValidActors) {
            // Try additional alternatives
            // 1. Check ULevel directly at various common offsets
            uint64_t levelActors = TargetProcess.Read<uint64_t>(PersistentLevel + 0xa0);
            printf("Level Actors (offset 0xa0): %p\n", levelActors);

            if (levelActors) {
                uint32_t levelActorCount = TargetProcess.Read<uint32_t>(PersistentLevel + 0xa8);
                printf("Level Actor Count: %d\n", levelActorCount);

                if (levelActorCount > 0 && levelActorCount < 10000) {
                    ActorArrayPtr = levelActors;
                    ActorCount = levelActorCount;
                    printf("Using ULevel direct actor array\n");
                    foundValidActors = true;
                }
            }
        }

        if (!foundValidActors) {
            return;
        }
    }
    else {
        // Read the first few offsets of the ActorCluster to find potential array patterns
        printf("Exploring ActorCluster structure:\n");
        for (int i = 0; i < 0x40; i += 8) {
            uint64_t value = TargetProcess.Read<uint64_t>(ActorCluster + i);
            if (value != 0) {
                printf("Offset 0x%x: %p\n", i, value);

                // Check if this might be a TArray pointer
                uint64_t arrayCheck = TargetProcess.Read<uint64_t>(value);
                if (arrayCheck != 0) {
                    printf("  Potential array at offset 0x%x\n", i);
                }
            }
        }

        // Try common TArray pattern
        uint64_t potentialArrayPtr = TargetProcess.Read<uint64_t>(ActorCluster);
        uint32_t potentialArrayCount = TargetProcess.Read<uint32_t>(ActorCluster + 0x8);

        printf("Potential Actor Array: %p\n", potentialArrayPtr);
        printf("Potential Actor Count: %d\n", potentialArrayCount);

        // If the count seems reasonable, use this approach
        if (potentialArrayPtr && potentialArrayCount > 0 && potentialArrayCount < 10000) {
            ActorArrayPtr = potentialArrayPtr;
            ActorCount = potentialArrayCount;
            printf("Using standard TArray pattern\n");
            foundValidActors = true;
        }
        else {
            // Try alternative offset (common in UE)
            ActorArrayPtr = TargetProcess.Read<uint64_t>(ActorCluster + 0x10);
            ActorCount = TargetProcess.Read<uint32_t>(ActorCluster + 0x18);

            printf("Alternative Actor Array: %p\n", ActorArrayPtr);
            printf("Alternative Actor Count: %d\n", ActorCount);

            if (ActorArrayPtr && ActorCount > 0 && ActorCount < 10000) {
                printf("Using alternative TArray pattern\n");
                foundValidActors = true;
            }
            else {
                printf("Could not find valid actor array\n");
                return;
            }
        }
    }

    // Proceed with reading actors if we found a valid array
    if (foundValidActors) {
        printf("Final Actor Array: %p\n", ActorArrayPtr);
        printf("Actor Count: %d\n", ActorCount);

        // Proceed with reading actors
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

        TargetProcess.ExecuteReadScatter(handle);
        TargetProcess.CloseScatterHandle(handle);

        handle = TargetProcess.CreateScatterHandle();
        for (std::shared_ptr<ActorEntity> entity : actors) {
            entity->SetUp1(handle);
        }

        TargetProcess.ExecuteReadScatter(handle);
        TargetProcess.CloseScatterHandle(handle);

        std::vector<std::shared_ptr<ActorEntity>> playerlist;
        for (std::shared_ptr<ActorEntity> entity : actors) {
            entity->SetUp2();
            if (entity->GetCharacterName() == LIT(L"Entity"))
                continue;
            if (entity->GetPosition() == Vector3::Zero())
                continue;
            playerlist.push_back(entity);
        }

        Actors = playerlist;
    }
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