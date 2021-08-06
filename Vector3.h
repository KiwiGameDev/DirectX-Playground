#pragma once
#include "Vector4.h"

struct Vector3
{
	Vector3() : x(0), y(0), z(0) { }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
	Vector3(const Vector4& vec4) : x(vec4.x), y(vec4.y), z(vec4.z) {}
	
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
	{
		Vector3 v;
		v.x = start.x * (1.0f - t) + end.x * t;
		v.y = start.y * (1.0f - t) + end.y * t;
		v.z = start.z * (1.0f - t) + end.z * t;
		return v;
	}

	Vector3 operator*(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}

	Vector3 operator+(Vector3 other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	
	float x;
	float y;
	float z;
};
