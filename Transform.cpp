#include "Transform.h"
#include "Mathf.h"

Transform::Transform(GameObject* owner)
	: Component(owner), position(0, 0, 0), orientation(0, 0, 0, 1), scale(1.0f, 1.0f, 1.0f)
{
	
}

void Transform::onStart()
{
	
}

void Transform::perform()
{
	
}

Matrix4x4 Transform::getTransformMatrix()
{
	if (is_transform_dirty)
	{
		Vector4 q(orientation.x, orientation.y, orientation.z, orientation.w);
		transform = Matrix4x4::scale(scale);
		transform *= Matrix4x4::rotation(q);
		transform *= Matrix4x4::translation(position);
		is_transform_dirty = false;
	}
	
	return transform;
}

Matrix4x4 Transform::getInverseTransformMatrix()
{
	if (is_inverse_transform_dirty)
	{
		inverse_transform = getTransformMatrix();
		inverse_transform.inverse();
		is_inverse_transform_dirty = false;
	}

	return inverse_transform;
}

void Transform::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transform::setPosition(Vector3 new_position)
{
	position = new_position;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transform::setOrientation(float x, float y, float z, float w)
{
	orientation = reactphysics3d::Quaternion(x, y, z, w);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transform::setOrientation(const reactphysics3d::Quaternion& new_orientation)
{
	orientation = new_orientation;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transform::setOrientationEuler(float x, float y, float z)
{
	orientation = reactphysics3d::Quaternion::fromEulerAngles({ x, y, z });
}

void Transform::setOrientationEuler(const Vector3& new_orientation)
{
	orientation = reactphysics3d::Quaternion::fromEulerAngles(new_orientation.x, new_orientation.y, new_orientation.z);
}

void Transform::setScale(float x, float y, float z)
{
	scale = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transform::setScale(Vector3 new_scale)
{
	scale = new_scale;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

Vector3 Transform::getPosition() const
{
	return position;
}

reactphysics3d::Quaternion Transform::getOrientation() const
{
	return orientation;
}

Vector3 Transform::getOrientationEuler() const
{
	return Mathf::getEulerFromQuaternion(orientation);
}

Vector3 Transform::getScale() const
{
	return scale;
}
