#pragma once
#include "Engine.h"

/**
 * Camera System
 * 
 * This class handles camera-related calculations and transformations.
 * It provides functionality for:
 * - Converting 3D world coordinates to 2D screen coordinates
 * - Handling perspective projection
 * - Managing view frustum calculations
 */
class Camera
{
public:
	/**
	 * Converts a 3D world position to 2D screen coordinates
	 * This is used for ESP and other screen-space rendering
	 * 
	 * @param viewinfo - Current camera view information
	 * @param world - 3D position in world space
	 * @return 2D screen coordinates, or Vector2::Zero() if point is behind camera
	 */
	static Vector2 WorldToScreen(MinimalViewInfo viewinfo, Vector3 world);
};