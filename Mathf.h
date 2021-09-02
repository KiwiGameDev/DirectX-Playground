#pragma once
#include <reactphysics3d/mathematics/Quaternion.h>

namespace Mathf
{
	static float PI = 3.141592654f;
	static float rad2deg = 57.2957795131f;
	static float deg2rad = 0.0174532925199f;

	static Vector3 getEulerFromQuaternion(reactphysics3d::Quaternion q)
	{
		Vector3 rot;
		float xSqr = q.x * q.x;
		float ySqr = q.y * q.y;
		float zSqr = q.z * q.z;
		
		// X
		rot.x = std::atan2(2.0f * (q.x * q.w + q.y * q.z), 1.0f - 2.0f * (xSqr + ySqr));

		// Y
		float sinp = 2.0f * (q.y * q.w - q.x * q.z);
		if (std::abs(sinp) >= 1.0f)
			rot.y = std::copysign(PI / 2.0f, sinp); // use 90 degrees if out of range
		else
			rot.y = std::asin(sinp);
		
		// Z
		rot.z = std::atan2(2.0f * (q.x * q.y + q.z * q.w), 1.0f - 2.0f * (ySqr + zSqr));

		return rot;
	}

	static float clamp(float value, float min, float max)
	{
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}

		return value;
	}
};
