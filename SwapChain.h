#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
	friend class DeviceContext;

public:
	SwapChain() = default;
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	bool release();
	~SwapChain() = default;

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
};
