#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class ITransformable
{
public:
	ITransformable();

	Matrix4x4 getTransform() const;
	Matrix4x4 getInverseTransform() const;
	
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
};
