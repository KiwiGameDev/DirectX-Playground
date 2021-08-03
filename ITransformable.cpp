#include "ITransformable.h"

ITransformable::ITransformable()
	: Position(0, 0, 0), Rotation(0, 0, 0), Scale(1.0f, 1.0f, 1.0f)
{
	
}

Matrix4x4 ITransformable::getTransform() const
{
	Matrix4x4 transform = Matrix4x4::identity();
	transform *= Matrix4x4::scale(Scale);
	transform *= Matrix4x4::rotationX(Rotation.x);
	transform *= Matrix4x4::rotationY(Rotation.y);
	transform *= Matrix4x4::rotationZ(Rotation.z);
	transform *= Matrix4x4::translation(Position);
	return transform;
}

Matrix4x4 ITransformable::getInverseTransform() const
{
	Matrix4x4 mat = getTransform();
	mat.inverse();
	return mat;
}
