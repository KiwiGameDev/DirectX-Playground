#include "PixelShader.h"
#include "RenderSystem.h"
#include <d3d11.h>
#include <exception>

PixelShader::PixelShader(const void* shader_byte_code, size_t size_byte_code, RenderSystem* system)
	: m_system(system)
{
	if (!SUCCEEDED(m_system->m_d3d_device->CreatePixelShader(shader_byte_code, size_byte_code, nullptr, &m_ps)))
		throw std::exception("Pixel Shader failed to initialize!");
}

PixelShader::~PixelShader()
{
	m_ps->Release();
}
