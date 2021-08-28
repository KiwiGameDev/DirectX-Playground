#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "SamplerState.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <exception>

RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	ID3D11DeviceContext* imm_context;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types; driver_type_index++)
	{
		res = D3D11CreateDevice(
			NULL,
			driver_types[driver_type_index],
			NULL,
			NULL,
			feature_levels,
			num_feature_levels,
			D3D11_SDK_VERSION,
			&m_d3d_device,
			&m_feature_level,
			&imm_context);

		if (SUCCEEDED(res))
			break;
	}

	if (FAILED(res))
		throw std::exception("Render System failed to initialize!");

	m_imm_device_context = std::make_shared<DeviceContext>(imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	// Rasterizers
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;
	m_d3d_device->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_wireframe);
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	m_d3d_device->CreateRasterizerState(&rasterizer_desc, &m_rasterizer_solid);

	// Sampler State
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	m_sampler_state = std::make_shared<SamplerState>(sampler_desc, this);
	m_imm_device_context->setSamplerState(m_sampler_state);
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;
	
	try
	{
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch(...)
	{
		
	}
	
	return sc;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertices, UINT size_list, VertexShaderPtr vertex_shader, VertexFormat vertex_format)
{
	VertexBufferPtr vb = nullptr;
	
	try
	{
		vb = std::make_shared<VertexBuffer>(list_vertices, size_vertices, size_list, vertex_shader, vertex_format, this);
	}
	catch (...)
	{

	}
	
	return vb;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr ib = nullptr;

	try
	{
		ib = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch (...)
	{

	}

	return ib;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr cb = nullptr;

	try
	{
		cb = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch (...)
	{

	}
	
	return cb;
}

void RenderSystem::setSolidRasterizerState()
{
	m_imm_device_context->setRasterizerState(m_rasterizer_solid);
}

void RenderSystem::setWireframeRasterizerState()
{
	m_imm_device_context->setRasterizerState(m_rasterizer_wireframe);
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return m_imm_device_context;
}

RenderSystem::~RenderSystem()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();
	m_d3d_device->Release();

	m_rasterizer_solid->Release();
	m_rasterizer_wireframe->Release();
}
