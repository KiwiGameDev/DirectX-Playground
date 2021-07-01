#include "VertexShader.h"
#include "RenderSystem.h"
#include <d3d11.h>
#include <exception>

VertexShader::VertexShader(const void* shader_byte_code, size_t size_byte_code, RenderSystem* system)
	: m_system(system)
{
	if (!SUCCEEDED(m_system->m_d3d_device->CreateVertexShader(shader_byte_code, size_byte_code, nullptr, &m_vs)))
		throw std::exception("Vertex Shader failed to initialize!");
}

VertexShader::~VertexShader()
{
	m_vs->Release();
}
