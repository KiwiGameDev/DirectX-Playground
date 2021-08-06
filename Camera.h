#pragma once
#include "IUpdateable.h"
#include "Transformable.h"
#include "Vector2.h"
#include "Point.h"

class Camera : public IUpdateable, public Transformable
{
public:
	Camera(float fov, float aspect, float near, float far);
	
	void update() override;

	void onKeyDown(int key);
	void onKeyUp(int key);
	void onMouseMove(const Vector2& window_size, const Point& mouse_pos);
	
	void setFOV(float new_fov);
	void setAspectRatio(float new_aspect);

	float getAspectRatio() const;
	float getFOV() const;
	float getFarZ() const;
	float getNearZ() const;

	Matrix4x4 getViewMatrix();
	Matrix4x4 getProjectionMatrix();

private:
	Matrix4x4 m_proj;
	
	bool m_is_proj_dirty = true;
	
	float m_fov;
	float m_ar;
	float m_near;
	float m_far;
	
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
};
