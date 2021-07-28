#include "Camera.h"

Camera::Camera()
{
	
}

Matrix4x4 Camera::getTransform() const
{
	Matrix4x4 transform = Matrix4x4::identity();
	transform *= Matrix4x4::rotationX(rotation.x);
	transform *= Matrix4x4::rotationY(rotation.y);
	transform *= Matrix4x4::rotationZ(rotation.z);
	transform *= Matrix4x4::translation(position);
	return transform;
}

void Camera::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
}

void Camera::setPosition(const Vector3& newPosition)
{
	position = newPosition;
}

Vector3 Camera::getLocalPosition() const
{
	return position;
}

void Camera::setRotation(float x, float y, float z)
{
	rotation = Vector3(x, y, z);
}

void Camera::setRotation(const Vector3& newRotation)
{
	rotation = newRotation;
}

void Camera::setRotationX(float x)
{
	rotation.x = x;
}

void Camera::setRotationY(float y)
{
	rotation.y = y;
}

void Camera::setRotationZ(float z)
{
	rotation.z = z;
}

float Camera::getLocalRotationX() const
{
	return rotation.x;
}

float Camera::getLocalRotationY() const
{
	return rotation.y;
}

float Camera::getLocalRotationZ() const
{
	return rotation.z;
}

Vector3 Camera::getLocalRotation() const
{
	return rotation;
}
