#pragma once
#include "Prerequisites.h"
#include <d3d11.h>

#include "VertexFormat.h"

class RenderSystem
{
	friend class SwapChain;
	friend class ConstantBuffer;
	friend class VertexBuffer;
	friend class VertexInputLayout;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;
	friend class DeviceContext;
	friend class UI;

public:
	RenderSystem();

	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertices, UINT size_list, VertexShaderPtr vertex_shader, VertexFormat vertex_format);
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	
	void setSolidRasterizerState();
	void setWireframeRasterizerState();

	DeviceContextPtr getImmediateDeviceContext();
	
	~RenderSystem();

private:
	DeviceContextPtr m_imm_device_context;
	
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;

	ID3D11RasterizerState* m_rasterizer_solid;
	ID3D11RasterizerState* m_rasterizer_wireframe;
};
