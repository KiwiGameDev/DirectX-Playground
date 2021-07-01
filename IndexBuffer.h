#pragma once
#include <Windows.h>

class RenderSystem;
class DeviceContext;
class ID3D11Buffer;
class ID3D11InputLayout;

class IndexBuffer
{
	friend class DeviceContext;

public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);

	UINT getSizeIndices();
	
	~IndexBuffer();

private:
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
	
	RenderSystem* m_system;
};
