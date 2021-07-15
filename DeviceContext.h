#pragma once
#include "Prerequisites.h"
#include <Windows.h>

class RenderSystem;
class GraphicsEngine;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class ID3D11DeviceContext;

class DeviceContext
{
	friend class GraphicsEngine;
	friend class ConstantBuffer;

public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	
	void setViewportSize(UINT width, UINT height);
	void clearRenderTarget(SwapChainPtr swap_chain, float r, float g, float b, float a);
	
	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& constant_buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& constant_buffer);
	void setTextureVertexShader(const TexturePtr& texture);
	void setTexturePixelShader(const TexturePtr& texture);
	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);
	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT start_vertex_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	~DeviceContext();

private:
	ID3D11DeviceContext* m_device_context;
	
	RenderSystem* m_system;
};
