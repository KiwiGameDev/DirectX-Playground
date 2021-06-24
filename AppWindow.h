#pragma once
#include "Window.h"

class PixelShader;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class VertexShader;

class AppWindow : public Window
{
public:
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;

private:
	SwapChain* m_swap_chain;
	ConstantBuffer* m_cb;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
};
