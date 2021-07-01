#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system)
	: m_system(system)
{
	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_buffer;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer)))
		throw std::exception("Constant Buffer failed to initialize!");
}

void ConstantBuffer::update(DeviceContext* device_context, void* buffer)
{
	device_context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, 0, 0);
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_buffer)
		m_buffer->Release();
}