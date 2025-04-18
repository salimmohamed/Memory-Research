#pragma once

/**
 * Engine Structures
 * 
 * This file defines the core data structures used by the Unreal Engine.
 * These structures are used to represent:
 * - 3D positions and rotations
 * - Camera information
 * - View calculations
 */

/**
 * 3D Vector structure used by Unreal Engine
 * Represents a point or direction in 3D space
 * @field X - X coordinate
 * @field Y - Y coordinate
 * @field Z - Z coordinate
 */
struct UEVector {
	double X; // 0x0(0x8)
	double Y; // 0x8(0x8)
	double Z; // 0x10(0x8)
};

/**
 * Rotation structure used by Unreal Engine
 * Represents orientation in 3D space using Euler angles
 * @field Pitch - Rotation around X axis (up/down)
 * @field Yaw - Rotation around Y axis (left/right)
 * @field Roll - Rotation around Z axis (tilt)
 */
struct UERotator {
	double Pitch; // 0x0(0x8)
	double Yaw; // 0x8(0x8)
	double Roll; // 0x10(0x8)
};

/**
 * Camera view information structure
 * Contains all data needed to represent a camera's view
 * @field Location - Camera position in world space
 * @field Rotation - Camera orientation
 * @field FOV - Field of view angle in degrees
 */
struct MinimalViewInfo
{
	struct UEVector Location; // 0x0(0x18)
	struct UERotator Rotation; // 0x18(0x18)
	float FOV; // 0x30(0x4)
};

/**
 * Cached camera information
 * Used to store and update camera state
 * @field Timestamp - Time when this cache was updated
 * @field POV - Current camera view information
 */
struct CameraCacheEntry
{
	float Timestamp; // 0x00(0x04)
	char pad_4[0xc]; // 0x04(0x0c)
	MinimalViewInfo POV; // 0x10(0x5e0)
};