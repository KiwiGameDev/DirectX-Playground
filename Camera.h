#pragma once
#include "GameObject.h"
#include "Vector2.h"
#include "Matrix4x4.h"

class Camera : public GameObject
{
public:
	Camera(const std::string& name, float fov, float aspect, float near, float far);
	
	void update();

	void onKeyDown(int key);
	void onKeyUp(int key);
	void onMouseMove(const Vector2& delta_pos);
	
	void setFOV(float new_fov);
	void setAspectRatio(float new_aspect);

	Matrix4x4 getViewMatrix();
	Matrix4x4 getProjectionMatrix();

private:
	Matrix4x4 m_proj;
	
	bool m_is_proj_dirty = true;
	
	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;
	
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
};
