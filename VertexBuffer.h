#pragma once
#include <Windows.h>

class DeviceContext;
class ID3D11Buffer;
class ID3D11InputLayout;

class VertexBuffer
{
	friend class DeviceContext;

public:
	VertexBuffer();

	bool load(void* list_vertices, UINT size_vertices, UINT size_list, void* shader_byte_code, UINT size_byte_shader);

	UINT getSizeVertices();
	
	bool release();

private:
	UINT m_size_vertices;
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
};
