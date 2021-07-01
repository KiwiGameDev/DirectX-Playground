#pragma once
#include <Windows.h>

class RenderSystem;
class DeviceContext;
class ID3D11Buffer;
class ID3D11InputLayout;

class VertexBuffer
{
	friend class DeviceContext;

public:
	VertexBuffer(void* list_vertices, UINT size_vertices, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);

	UINT getSizeVertices();

	~VertexBuffer();

private:
	UINT m_size_vertices;
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;

	RenderSystem* m_system;
};
