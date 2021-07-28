#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

class Camera
{
public:
	Camera();

	Matrix4x4 getTransform() const;
	
	void setPosition(float x, float y, float z);
	void setPosition(const Vector3& newPosition);
	Vector3 getLocalPosition() const;

	void setRotation(float x, float y, float z);
	void setRotation(const Vector3& newRotation);
	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);
	float getLocalRotationX() const;
	float getLocalRotationY() const;
	float getLocalRotationZ() const;
	Vector3 getLocalRotation() const;

private:
	Vector3 position;
	Vector3 rotation;
};
