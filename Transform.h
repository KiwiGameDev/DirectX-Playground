#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include <reactphysics3d/mathematics/Quaternion.h>

class Transform : public Component
{
public:
	Transform(GameObject* owner);
	
	void perform() override;
	
	Matrix4x4 getTransformMatrix();
	Matrix4x4 getInverseTransformMatrix();
	
	void setPosition(float x, float y, float z);
	void setPosition(Vector3 new_position);
	void setOrientation(float x, float y, float z, float w);
	void setOrientation(const reactphysics3d::Quaternion& new_orientation);
	void setOrientationEuler(float x, float y, float z);
	void setOrientationEuler(const Vector3& new_orientation);
	void setScale(float x, float y, float z);
	void setScale(Vector3 new_scale);
	
	Vector3 getPosition() const;
	reactphysics3d::Quaternion getOrientation() const;
	Vector3 getOrientationEuler() const;
	Vector3 getScale() const;

protected:
	void onStart() override;
	
private:
	bool is_transform_dirty = true;
	bool is_inverse_transform_dirty = true;
	
	Matrix4x4 transform;
	Matrix4x4 inverse_transform;
	
	Vector3 position;
	reactphysics3d::Quaternion orientation;
	Vector3 scale;
};
