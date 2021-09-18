#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system)
	: m_system(system)
{
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = sizeof(int) * size_list;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
		throw std::exception("Index Buffer failed to initialize!");
}

UINT IndexBuffer::getIndicesCount()
{
	return m_size_list;
}

IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}
