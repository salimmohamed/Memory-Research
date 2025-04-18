#pragma once

/**
 * 2D vector class
 * Provides basic 2D vector operations and utilities
 */
class Vector2
{
public:
	/**
	 * Default constructor
	 * Initializes vector to (0,0)
	 */
	Vector2();

	/**
	 * Constructor with initial values
	 * @param _x - X component
	 * @param _y - Y component
	 */
	Vector2(float _x, float _y);

	/**
	 * Default destructor
	 */
	~Vector2();

	float x, y;  // Vector components

	/**
	 * Vector multiplication
	 * @param a - Vector to multiply with
	 * @return Resulting vector
	 */
	Vector2 operator *(Vector2 a);

	/**
	 * Vector division
	 * @param a - Vector to divide by
	 * @return Resulting vector
	 */
	Vector2 operator /(Vector2 a);

	/**
	 * Vector addition
	 * @param a - Vector to add
	 * @return Resulting vector
	 */
	Vector2 operator +(Vector2 a);

	/**
	 * Vector subtraction
	 * @param a - Vector to subtract
	 * @return Resulting vector
	 */
	Vector2 operator -(Vector2 a);

	/**
	 * Vector equality
	 * @param a - Vector to compare with
	 * @return true if vectors are equal
	 */
	bool operator ==(Vector2 a);

	/**
	 * Vector inequality
	 * @param a - Vector to compare with
	 * @return true if vectors are not equal
	 */
	bool operator !=(Vector2 a);

	/**
	 * Checks if vector is zero
	 * @return true if both components are zero
	 */
	bool IsZero() const;

	/**
	 * Gets zero vector
	 * @return Vector (0,0)
	 */
	static Vector2 Zero();

	/**
	 * Calculates distance between two vectors
	 * @param a - First vector
	 * @param b - Second vector
	 * @return Distance between vectors
	 */
	static float Distance(Vector2 a, Vector2 b);
};

/**
 * 3D vector class
 * Provides 3D vector operations and utilities
 */
class Vector3
{
public:
	/**
	 * Default constructor
	 * Initializes vector to (0,0,0)
	 */
	Vector3();

	/**
	 * Constructor with initial values
	 * @param _x - X component
	 * @param _y - Y component
	 * @param _z - Z component
	 */
	Vector3(float _x, float _y, float _z);

	/**
	 * Default destructor
	 */
	~Vector3();

	float x, y, z;  // Vector components

	/**
	 * Vector multiplication
	 * @param a - Vector to multiply with
	 * @return Resulting vector
	 */
	Vector3 operator *(Vector3 a);

	/**
	 * Scalar multiplication
	 * @param f - Scalar to multiply with
	 * @return Resulting vector
	 */
	Vector3 operator *(float f);

	/**
	 * Vector division
	 * @param a - Vector to divide by
	 * @return Resulting vector
	 */
	Vector3 operator /(Vector3 a);

	/**
	 * Scalar division
	 * @param f - Scalar to divide by
	 * @return Resulting vector
	 */
	Vector3 operator /(float f);

	/**
	 * Vector addition
	 * @param a - Vector to add
	 * @return Resulting vector
	 */
	Vector3 operator +(Vector3 a);

	/**
	 * Vector subtraction
	 * @param a - Vector to subtract
	 * @return Resulting vector
	 */
	Vector3 operator -(Vector3 a);

	/**
	 * Vector equality
	 * @param a - Vector to compare with
	 * @return true if vectors are equal
	 */
	bool operator ==(Vector3 a);

	/**
	 * Vector inequality
	 * @param a - Vector to compare with
	 * @return true if vectors are not equal
	 */
	bool operator !=(Vector3 a);

	/**
	 * Checks if vector is zero
	 * @return true if all components are zero
	 */
	bool IsZero() const;

	/**
	 * Calculates dot product of two vectors
	 * @param left - First vector
	 * @param right - Second vector
	 * @return Dot product
	 */
	static float Dot(Vector3 left, Vector3 right);

	/**
	 * Calculates distance between two vectors
	 * @param a - First vector
	 * @param b - Second vector
	 * @return Distance between vectors
	 */
	static float Distance(Vector3 a, Vector3 b);

	/**
	 * Calculates rounded distance between two vectors
	 * @param a - First vector
	 * @param b - Second vector
	 * @return Distance rounded to nearest 5 units
	 */
	static int FormattedDistance(Vector3 a, Vector3 b);

	/**
	 * Gets zero vector
	 * @return Vector (0,0,0)
	 */
	static Vector3 Zero();

	/**
	 * Linear interpolation between two vectors
	 * @param a - Start vector
	 * @param b - End vector
	 * @param t - Interpolation factor (0-1)
	 * @return Interpolated vector
	 */
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);

	/**
	 * Calculates vector length
	 * @return Vector length
	 */
	float Length() const;

	/**
	 * Calculates squared vector length
	 * @return Squared vector length
	 */
	float LengthSqr() const;

	/**
	 * Clamps vector angles to valid ranges
	 * @return Clamped vector
	 */
	Vector3 Clamp() const;
};

/**
 * View matrix structure
 * Used for 3D transformations
 */
struct ViewMatrix
{
public:
	float matrix[4][4];  // 4x4 transformation matrix

	/**
	 * Transforms a 3D point using the view matrix
	 * @param vector - Point to transform
	 * @return Transformed point
	 */
	Vector3 Transform(const Vector3 vector) const;
};
