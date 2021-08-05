#pragma once
#include "Window.h"
#include "IInputListener.h"
#include "Cube.h"
#include "Camera.h"
#include "FrustumGO.h"
#include "Prerequisites.h"
#include <vector>

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
	Camera m_game_camera;
	FrustumGO* m_frustum;
	
	std::vector<Cube> cubes;
	std::vector<GameObject> gameobjects;

	float m_timer = 0.0f;
};
