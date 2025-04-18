#pragma once
#include "pch.h"
#include "ActorEntity.h"
#include "EngineStructs.h"
#include <sstream>

/**
 * Stream operator overload for EPlayerRole
 * Converts the role enum to a human-readable string
 */
inline std::wostream& operator<<(std::wostream& os, EPlayerRole role) {
    switch (role) {
        case EPlayerRole::EDeceitRole__Human: os << L"Human"; break;
        case EPlayerRole::EDeceitRole__Terror: os << L"Terror"; break;
        case EPlayerRole::EDeceitRole__Medic: os << L"Medic"; break;
        case EPlayerRole::EDeceitRole__Warden: os << L"Warden"; break;
        case EPlayerRole::EDeceitRole__Inquisitor: os << L"Inquisitor"; break;
        case EPlayerRole::EDeceitRole__Mimic: os << L"Mimic"; break;
        case EPlayerRole::EDeceitRole__Vector: os << L"Vector"; break;
        case EPlayerRole::EDeceitRole__Seer: os << L"Seer"; break;
        case EPlayerRole::EDeceitRole__Purifier: os << L"Purifier"; break;
        case EPlayerRole::EDeceitRole__Medium: os << L"Medium"; break;
        case EPlayerRole::EDeceitRole__Chemist: os << L"Chemist"; break;
        case EPlayerRole::EDeceitRole__Soulbound: os << L"Soulbound"; break;
        case EPlayerRole::EDeceitRole__Phantom: os << L"Phantom"; break;
        case EPlayerRole::EDeceitRole__Invalid: os << L"Invalid"; break;
        default: os << L"Unknown"; break;
    }
    return os;
}

/**
 * Stream operator overload for CameraCacheEntry
 * Outputs the camera's position, rotation, and FOV
 */
inline std::wostream& operator<<(std::wostream& os, const CameraCacheEntry& entry) {
    os << L"CameraCacheEntry {\n"
       << L"  Timestamp: " << entry.Timestamp << L"\n"
       << L"  Location: (" << entry.POV.Location.X << L", " 
       << entry.POV.Location.Y << L", " 
       << entry.POV.Location.Z << L")\n"
       << L"  Rotation: (" << entry.POV.Rotation.Pitch << L", "
       << entry.POV.Rotation.Yaw << L", "
       << entry.POV.Rotation.Roll << L")\n"
       << L"  FOV: " << entry.POV.FOV << L"\n"
       << L"}";
    return os;
} 