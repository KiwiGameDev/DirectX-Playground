#pragma once
#include <d3d11.h>

class RenderSystem;
class DeviceContext;

class SwapChain
{
	friend class DeviceContext;

public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);

	bool present(bool vsync);

	~SwapChain();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;
	
	RenderSystem* m_system;
};
