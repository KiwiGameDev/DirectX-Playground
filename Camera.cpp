#include "Camera.h"
#include "Time.h"

Camera::Camera(float fov, float aspect, float near, float far)
	: m_fov(fov), m_ar(aspect), m_near(near), m_far(far)
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

void Camera::onMouseMove(const Vector2& window_size, const Point& mouse_pos)
{
	float window_width_half = window_size.x / 2.0f;
	float window_height_half = window_size.y / 2.0f;

	Vector3 rotation = getRotation();
	rotation.x += (mouse_pos.y - window_height_half) * Time::get().deltaTime() * 4.0f;
	rotation.y += (mouse_pos.x - window_width_half) * Time::get().deltaTime() * 4.0f;
	setRotation(rotation);
}

void Camera::setFOV(float new_fov)
{
	m_fov = new_fov;
	m_is_proj_dirty = true;
}

void Camera::setAspectRatio(float new_aspect)
{
	m_ar = new_aspect;
	m_is_proj_dirty = true;
}

float Camera::getAspectRatio() const
{
	return m_ar;
}

float Camera::getFOV() const
{
	return m_fov;
}

float Camera::getFarZ() const
{
	return m_far;
}

float Camera::getNearZ() const
{
	return m_near;
}

Matrix4x4 Camera::getViewMatrix()
{
	return getInverseTransform();
}

Matrix4x4 Camera::getProjectionMatrix()
{
	if (m_is_proj_dirty)
	{
		m_proj = Matrix4x4::perspectiveFovLH(m_fov, m_ar, m_near, m_far);
		m_is_proj_dirty = false;
	}

	return m_proj;
}