#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertices, UINT size_list, VertexShaderPtr vertex_shader, VertexFormat vertex_format, RenderSystem* system)
	: m_system(system), m_buffer(nullptr), m_layout(vertex_format, vertex_shader, system)
{
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

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
		throw std::exception("Vertex Buffer failed to initialize!");
}

UINT VertexBuffer::getSizeVertices()
{
	return m_size_vertices;
}

VertexBuffer::~VertexBuffer()
{
	m_buffer->Release();
}
