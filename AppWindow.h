#pragma once
#include "Window.h"

class IndexBuffer;
class PixelShader;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class VertexShader;

class AppWindow : public Window
{
public:
	void updateQuadPosition();
	
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;

private:
	SwapChain* m_swap_chain;
	ConstantBuffer* m_cb;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;

	float m_old_delta;
	float m_new_delta;
	float m_delta_time;
	float m_delta_pos;
	float m_delta_scale;
};
