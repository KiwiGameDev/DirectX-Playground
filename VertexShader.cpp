#include "VertexShader.h"
#include "GraphicsEngine.h"
#include <d3d11.h>

bool VertexShader::init(const void* shader_byte_code, size_t size_byte_code)
{
	if (!SUCCEEDED(GraphicsEngine::get().m_d3d_device->CreateVertexShader(shader_byte_code, size_byte_code, nullptr, &m_vs)))
		return false;

	return true;
}

void VertexShader::release()
{
	m_vs->Release();
	delete this;
}