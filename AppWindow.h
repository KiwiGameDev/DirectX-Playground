#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Matrix4x4.h"
#include "Prerequisites.h"

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
	static long long getMicrosecondsFromStart();
	
	SwapChainPtr m_swap_chain;
	ConstantBufferPtr m_cb;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	long long m_old_delta = 0LL;
	long long m_new_delta = 0LL;
	float m_delta_time = 0.0f;
	float m_delta_pos;
	float m_delta_scale;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;
	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world_camera;
};
