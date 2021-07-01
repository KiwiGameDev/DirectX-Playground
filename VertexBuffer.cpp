#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer()
	: m_buffer(nullptr), m_layout(nullptr)
{
	
}

bool VertexBuffer::load(void* list_vertices, UINT size_vertices, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	if (m_buffer)
		m_buffer->Release();
	if (m_layout)
		m_layout->Release();
	
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_vertices * size_list;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertices = size_vertices;
	m_size_list = size_list;

	if (FAILED(GraphicsEngine::get().m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
		return false;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT size_layout = ARRAYSIZE(layout);
	
	if (FAILED(GraphicsEngine::get().m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
		return false;

	return true;
}

UINT VertexBuffer::getSizeVertices()
{
	return m_size_vertices;
}

bool VertexBuffer::release()
{
	m_buffer->Release();
	m_layout->Release();
	delete this;
	return true;
}
