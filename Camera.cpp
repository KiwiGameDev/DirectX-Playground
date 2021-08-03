#include "Camera.h"
#include "Time.h"

void Camera::update()
{
	Matrix4x4 camera_transform = getTransform();
	Position = Position + camera_transform.getZDirection() * m_forward * 4.0f * Time::get().deltaTime() + camera_transform.getXDirection() * m_rightward * 4.0f * Time::get().deltaTime();
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
	
	Rotation.x += (mouse_pos.y - window_height_half) * Time::get().deltaTime() * 4.0f;
	Rotation.y += (mouse_pos.x - window_width_half) * Time::get().deltaTime() * 4.0f;
}
