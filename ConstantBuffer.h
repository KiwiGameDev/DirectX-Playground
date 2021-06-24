#pragma once
#include <Windows.h>

class DeviceContext;
class ID3D11Buffer;
class ID3D11InputLayout;

class ConstantBuffer
{
	friend class DeviceContext;

public:
	ConstantBuffer();

	bool load(void* buffer, UINT size_buffer);

	void update(DeviceContext* device_context, void* buffer);
	
	bool release();

private:
	ID3D11Buffer* m_buffer;
};
