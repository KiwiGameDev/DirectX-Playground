#pragma once
#include <Windows.h>

class ID3D11DeviceContext;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class VertexShader;
class PixelShader;
class GraphicsEngine;

class DeviceContext
{
	friend class GraphicsEngine;
	friend class ConstantBuffer;

public:
	DeviceContext(ID3D11DeviceContext* device_context);
	
	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* constant_buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* constant_buffer);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void clearRenderTarget(SwapChain* swap_chain, float r, float g, float b, float a);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	bool release();

private:
	ID3D11DeviceContext* m_device_context;
};
