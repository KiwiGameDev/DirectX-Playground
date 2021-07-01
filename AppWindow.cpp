#include "AppWindow.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "PixelShader.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include <Windows.h>

struct vec3
{
	float x;
	float y;
	float z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	unsigned int m_time;
};

void AppWindow::onCreate()
{
	Window::onCreate();
	
	GraphicsEngine::get().init();

	m_swap_chain = GraphicsEngine::get().createSwapChain();
	RECT rect = getClientWindowRect();
	m_swap_chain->init(m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// Temporary create triangle
	vertex list_triangle[] =
	{
		{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f},
		{ 0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f}
	};
	UINT size_list = ARRAYSIZE(list_triangle);

	m_vb = GraphicsEngine::get().createVertexBuffer();
	
	void* shader_byte_code = nullptr;
	size_t size_shader_byte_code = 0;
	GraphicsEngine::get().compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader_byte_code);
	m_vs = GraphicsEngine::get().createVertexShader(shader_byte_code, size_shader_byte_code);
	m_vb->load(list_triangle, sizeof(vertex), size_list, shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().releaseCompiledShader();
	
	GraphicsEngine::get().compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader_byte_code);
	m_ps = GraphicsEngine::get().createPixelShader(shader_byte_code, size_shader_byte_code);
	GraphicsEngine::get().releaseCompiledShader();

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get().createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	GraphicsEngine::get().getImmediateDeviceContext()->clearRenderTarget(m_swap_chain, 0.1f, 0.1f, 0.1f, 1.0f);

	RECT rect = getClientWindowRect();
	constant cc;
	cc.m_time = GetTickCount();

	m_cb->update(GraphicsEngine::get().getImmediateDeviceContext(), &cc);

	GraphicsEngine::get().getImmediateDeviceContext()->setViewportSize(rect.right - rect.left, rect.bottom - rect.top);
	GraphicsEngine::get().getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);;
	GraphicsEngine::get().getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);;
	GraphicsEngine::get().getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get().getImmediateDeviceContext()->setPixelShader(m_ps);
	GraphicsEngine::get().getImmediateDeviceContext()->setVertexBuffer(m_vb);
	
	GraphicsEngine::get().getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertices(), 0);

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get().release();
}
