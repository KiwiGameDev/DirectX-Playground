#include "PixelShader.h"
#include "GraphicsEngine.h"
#include <d3d11.h>

bool PixelShader::init(const void* shader_byte_code, size_t size_byte_code)
{
	if (!SUCCEEDED(GraphicsEngine::get().m_d3d_device->CreatePixelShader(shader_byte_code, size_byte_code, nullptr, &m_ps)))
		return false;

	return true;
}

void PixelShader::release()
{
	m_ps->Release();
	delete this;
}