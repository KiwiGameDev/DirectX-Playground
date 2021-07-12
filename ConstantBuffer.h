#pragma once
#include <Windows.h>
#include "Prerequisites.h"

class RenderSystem;
class ID3D11Buffer;
class ID3D11InputLayout;

class ConstantBuffer
{
	friend class DeviceContext;

public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);

	void update(DeviceContextPtr device_context, void* buffer);

	~ConstantBuffer();

private:
	ID3D11Buffer* m_buffer;
	
	RenderSystem* m_system;
};
