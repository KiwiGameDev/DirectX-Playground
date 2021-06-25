#pragma once
#include <Windows.h>

class DeviceContext;
class ID3D11Buffer;
class ID3D11InputLayout;

class IndexBuffer
{
	friend class DeviceContext;

public:
	IndexBuffer();

	bool load(void* list_indices, UINT size_list);

	UINT getSizeIndices();

	bool release();

private:
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
};
