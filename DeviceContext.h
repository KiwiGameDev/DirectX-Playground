#pragma once
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
	
	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* constant_buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* constant_buffer);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setIndexBuffer(IndexBuffer* index_buffer);
	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void clearRenderTarget(SwapChain* swap_chain, float r, float g, float b, float a);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT start_vertex_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	~DeviceContext();

private:
	ID3D11DeviceContext* m_device_context;
	
	RenderSystem* m_system;
};
