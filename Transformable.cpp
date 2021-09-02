#include "Transformable.h"
#include "Mathf.h"

Transformable::Transformable()
	: position(0, 0, 0), orientation(0, 0, 0, 1), scale(1.0f, 1.0f, 1.0f)
{
	
}

Matrix4x4 Transformable::getTransform()
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

Matrix4x4 Transformable::getInverseTransform()
{
	if (is_inverse_transform_dirty)
	{
		inverse_transform = getTransform();
		inverse_transform.inverse();
		is_inverse_transform_dirty = false;
	}

	return inverse_transform;
}

void Transformable::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transformable::setPosition(Vector3 new_position)
{
	position = new_position;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transformable::setOrientation(float x, float y, float z, float w)
{
	orientation = reactphysics3d::Quaternion(x, y, z, w);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transformable::setOrientation(const reactphysics3d::Quaternion& new_orientation)
{
	orientation = new_orientation;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transformable::setOrientationEuler(float x, float y, float z)
{
	orientation = reactphysics3d::Quaternion::fromEulerAngles({ x, y, z });
}

void Transformable::setOrientationEuler(const Vector3& new_orientation)
{
	orientation = reactphysics3d::Quaternion::fromEulerAngles(new_orientation.x, new_orientation.y, new_orientation.z);
}

void Transformable::setScale(float x, float y, float z)
{
	scale = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

void Transformable::setScale(Vector3 new_scale)
{
	scale = new_scale;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
}

Matrix4x4 Transformable::getTransform() const
{
	return transform;
}

Vector3 Transformable::getPosition() const
{
	return position;
}

reactphysics3d::Quaternion Transformable::getOrientation() const
{
	return orientation;
}

Vector3 Transformable::getOrientationEuler() const
{
	return Mathf::getEulerFromQuaternion(orientation);
}

Vector3 Transformable::getScale() const
{
	return scale;
}
