#include "ITransformable.h"

ITransformable::ITransformable()
	: position(0, 0, 0), rotation(0, 0, 0), scale(1.0f, 1.0f, 1.0f)
{
	
}

Matrix4x4 ITransformable::getTransform()
{
	if (is_transform_dirty)
	{
		transform = Matrix4x4::identity();
		transform *= Matrix4x4::scale(scale);
		transform *= Matrix4x4::rotationX(rotation.x);
		transform *= Matrix4x4::rotationY(rotation.y);
		transform *= Matrix4x4::rotationZ(rotation.z);
		transform *= Matrix4x4::translation(position);
		is_transform_dirty = false;
	}
	
	return transform;
}

Matrix4x4 ITransformable::getInverseTransform()
{
	if (is_inverse_transform_dirty)
	{
		inverse_transform = getTransform();
		inverse_transform.inverse();
		is_inverse_transform_dirty = false;
	}

	return inverse_transform;
}

Vector3 ITransformable::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return position;
}

Vector3 ITransformable::setPosition(Vector3 new_position)
{
	position = new_position;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return position;
}

Vector3 ITransformable::setRotation(float x, float y, float z)
{
	rotation = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return rotation;
}

Vector3 ITransformable::setRotation(Vector3 new_rotation)
{
	rotation = new_rotation;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return rotation;
}

Vector3 ITransformable::setScale(float x, float y, float z)
{
	scale = Vector3(x, y, z);
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return scale;
}

Vector3 ITransformable::setScale(Vector3 new_scale)
{
	scale = new_scale;
	is_transform_dirty = true;
	is_inverse_transform_dirty = true;
	return scale;
}

Vector3 ITransformable::getPosition() const
{
	return position;
}

Vector3 ITransformable::getRotation() const
{
	return rotation;
}

Vector3 ITransformable::getScale() const
{
	return scale;
}
