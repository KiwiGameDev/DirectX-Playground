#include "Camera.h"
#include "Transform.h"
#include "Mathf.h"
#include "Time.h"

Camera::Camera(const std::string& name, float fov, float aspect, float near, float far)
	: GameObject(name), m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
{
	m_proj = Matrix4x4::perspectiveFovLH(fov, aspect, near, far);
}

void Camera::update()
{
	GameObject::update();
	
	Matrix4x4 camera_transform = getComponent<Transform>().getTransformMatrix();
	Vector3 move_z = camera_transform.getZDirection() * m_forward * 4.0f * Time::get().deltaTime();
	Vector3 move_x = camera_transform.getXDirection() * m_rightward * 4.0f * Time::get().deltaTime();
	getComponent<Transform>().setPosition(getComponent<Transform>().getPosition() + move_z + move_x);
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
	Transform transform = getComponent<Transform>();
	reactphysics3d::Quaternion q = transform.getOrientation();

	Vector3 rightDir = transform.getTransformMatrix().getXDirection();
	float yRotAngleHalf = Mathf::clamp(delta_pos.x * Time::get().deltaTime() * 4.0f, -0.1f, 0.1f);
	reactphysics3d::Quaternion yRot(0.0f, sinf(yRotAngleHalf), 0.0f, cosf(yRotAngleHalf));
	
	float xRotAngleHalf = Mathf::clamp(delta_pos.y * Time::get().deltaTime() * 4.0f, -0.1f, 0.1f);
	float xRotSinAngleHalf = sinf(xRotAngleHalf);
	reactphysics3d::Quaternion xRot(rightDir.x * xRotSinAngleHalf, rightDir.y * xRotSinAngleHalf, rightDir.z * xRotSinAngleHalf, cosf(xRotAngleHalf));

	q = xRot * q;
	q = yRot * q;
	
	getComponent<Transform>().setOrientation(q);
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
	return getComponent<Transform>().getInverseTransformMatrix();
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
