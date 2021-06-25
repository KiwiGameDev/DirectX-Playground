#pragma once

struct Vector3
{
	Vector3() : x(0), y(0), z(0) { }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
	Vector3(const Vector3& copy) : x(copy.x), y(copy.y), z(copy.z) { }

	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
	{
		Vector3 v;
		v.x = start.x * (1.0f - t) + end.x * t;
		v.y = start.y * (1.0f - t) + end.y * t;
		v.z = start.z * (1.0f - t) + end.z * t;
		return v;
	}
	
	float x;
	float y;
	float z;
};