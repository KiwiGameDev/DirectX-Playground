#include "PixelShader.h"
#include "RenderSystem.h"
#include <d3dcompiler.h>
#include <exception>

PixelShader::PixelShader(const wchar_t* full_path, RenderSystem* system)
	: m_system(system)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(D3DCompileFromFile(full_path, nullptr, nullptr, "main", "ps_5_0", 0, 0, &blob, &error_blob)))
	{
		if (error_blob)
			error_blob->Release();

		throw std::exception("Failed to compile Vertex Shader!");
	}

	m_shader_byte_code_size = blob->GetBufferSize();
	m_shader_byte_code = malloc(m_shader_byte_code_size);
	memcpy(m_shader_byte_code, blob->GetBufferPointer(), m_shader_byte_code_size);
	
	if (!SUCCEEDED(m_system->m_d3d_device->CreatePixelShader(m_shader_byte_code, m_shader_byte_code_size, nullptr, &m_ps)))
		throw std::exception("Vertex Shader failed to initialize!");

	blob->Release();
}

PixelShader::~PixelShader()
{
	m_ps->Release();
	free(m_shader_byte_code);
}
