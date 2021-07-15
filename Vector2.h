#pragma once

struct Vector2
{
	Vector2() : x(0), y(0) { }
	Vector2(float x, float y) : x(x), y(y) { }

	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
	{
		Vector2 v;
		v.x = start.x * (1.0f - t) + end.x * t;
		v.y = start.y * (1.0f - t) + end.y * t;
		return v;
	}

	Vector2 operator*(float num)
	{
		return Vector2(x * num, y * num);
	}

	Vector2 operator+(Vector2 other)
	{
		return Vector2(x + other.x, y + other.y);
	}

	float x;
	float y;
};