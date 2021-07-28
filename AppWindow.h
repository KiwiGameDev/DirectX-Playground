#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Cube.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"
#include "Camera.h"
#include <vector>

class AppWindow : public Window, public IInputListener
{
public:
	void update();
	
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;
	
	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& delta_mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;

private:
	SwapChainPtr m_swap_chain;
	ConstantBufferPtr m_cb;

	std::vector<Cube> cubes;
	std::vector<GameObject> gameobjects;
	
	float m_timer = 0.0f;
	float m_delta_time_multiplier = 1.0f;
	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Camera m_camera;
};
