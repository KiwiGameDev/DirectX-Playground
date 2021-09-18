#include "VertexShader.h"
#include "RenderSystem.h"
#include <d3dcompiler.h>
#include <exception>

VertexShader::VertexShader(const std::string& file_path, RenderSystem& render_system)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(D3DCompileFromFile(std::wstring(file_path.begin(), file_path.end()).c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		if (error_blob)
			error_blob->Release();

		throw std::exception("Failed to compile Vertex Shader!");
	}
	
	m_shader_byte_code_size = blob->GetBufferSize();
	m_shader_byte_code = malloc(m_shader_byte_code_size);
	memcpy(m_shader_byte_code, blob->GetBufferPointer(), m_shader_byte_code_size);
	
	if (!SUCCEEDED(render_system.m_d3d_device->CreateVertexShader(m_shader_byte_code, m_shader_byte_code_size, nullptr, &m_vs)))
		throw std::exception("Vertex Shader failed to initialize!");

	blob->Release();
}

VertexShader::~VertexShader()
{
	m_vs->Release();
	free(m_shader_byte_code);
}
