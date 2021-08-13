#include "Camera.h"
#include "Time.h"

Camera::Camera(float fov, float aspect, float near, float far)
	: m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{
	m_proj = Matrix4x4::perspectiveFovLH(fov, aspect, near, far);
}

void Camera::update()
{
	Matrix4x4 camera_transform = getTransform();
	Vector3 move_z = camera_transform.getZDirection() * m_forward * 4.0f * Time::get().deltaTime();
	Vector3 move_x = camera_transform.getXDirection() * m_rightward * 4.0f * Time::get().deltaTime();
	setPosition(getPosition() + move_z + move_x);
}

void Camera::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void Camera::onKeyUp(int key)
{
	if (key == 'W' || key == 'S')
	{
		m_forward = 0.0f;
	}
	else if (key == 'A' || key == 'D')
	{
		m_rightward = 0.0f;
	}
}

void Camera::onMouseMove(const Vector2& delta_pos)
{
	Vector3 rotation = getRotation();
	rotation.x += delta_pos.y * Time::get().deltaTime() * 4.0f;
	rotation.y += delta_pos.x * Time::get().deltaTime() * 4.0f;
	setRotation(rotation);
}

void Camera::setFOV(float new_fov)
{
	m_fov = new_fov;
	m_is_proj_dirty = true;
}

void Camera::setAspectRatio(float new_aspect)
{
	m_aspect = new_aspect;
	m_is_proj_dirty = true;
}

Matrix4x4 Camera::getViewMatrix()
{
	return getInverseTransform();
}

Matrix4x4 Camera::getProjectionMatrix()
{
	if (m_is_proj_dirty)
	{
		m_proj = Matrix4x4::perspectiveFovLH(m_fov, m_aspect, m_near, m_far);
		m_is_proj_dirty = false;
	}

	return m_proj;
}
