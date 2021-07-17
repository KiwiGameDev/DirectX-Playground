#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Prerequisites.h"

class Mesh : public Resource
{
	friend class DeviceContext;

public:
	Mesh(const wchar_t* full_path);

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	
	~Mesh();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
};
