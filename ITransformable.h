#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class ITransformable
{
public:
	ITransformable();

	Matrix4x4 getTransform();
	Matrix4x4 getInverseTransform();

	Vector3 setPosition(float x, float y, float z);
	Vector3 setPosition(Vector3 new_position);
	Vector3 setRotation(float x, float y, float z);
	Vector3 setRotation(Vector3 new_rotation);
	Vector3 setScale(float x, float y, float z);
	Vector3 setScale(Vector3 new_scale);

	Vector3 getPosition() const;
	Vector3 getRotation() const;
	Vector3 getScale() const;

private:
	bool is_transform_dirty = true;
	bool is_inverse_transform_dirty = true;
	
	Matrix4x4 transform;
	Matrix4x4 inverse_transform;
	
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
