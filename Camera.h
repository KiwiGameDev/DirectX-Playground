#pragma once
#include "IUpdateable.h"
#include "ITransformable.h"
#include "Vector2.h"
#include "Point.h"

class Camera : public IUpdateable, public ITransformable
{
public:
	void update() override;

	void onKeyDown(int key);
	void onKeyUp(int key);
	void onMouseMove(const Vector2& window_size, const Point& mouse_pos);

private:
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
};
