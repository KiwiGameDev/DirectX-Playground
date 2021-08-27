#include "VertexInputLayout.h"
#include "RenderSystem.h"
#include "VertexShader.h"
#include <d3d11.h>
#include <vector>
#include <exception>

VertexInputLayout::VertexInputLayout(VertexFormat vertex_format, VertexShaderPtr vertex_shader, RenderSystem* render_system)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout_desc;

	if (vertex_format == VertexFormat::POSITION_COLOR)
	{
		layout_desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout_desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else if (vertex_format == VertexFormat::POSITION_UV)
	{
		layout_desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		layout_desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	else
	{
		throw std::exception("Invalid Vertex Format!");
	}

	if (FAILED(render_system->m_d3d_device->CreateInputLayout(layout_desc.data(), (UINT)layout_desc.size(), vertex_shader->m_shader_byte_code, vertex_shader->m_shader_byte_code_size, &m_layout)))
		throw std::exception("Input Layout failed to initialize!");
}

VertexInputLayout::~VertexInputLayout()
{
	m_layout->Release();
}
