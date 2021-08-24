#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Cube.h"
#include "Camera.h"
#include "Prerequisites.h"

class AppWindow : public Window, public IInputListener
{
public:
	AppWindow();
	
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;

private:
	SwapChainPtr m_swap_chain;
	ConstantBufferPtr m_cb;

	Camera m_editor_camera;
	
	bool m_is_mouse_locked = false;
	Point m_mouse_locked_pos;
};
