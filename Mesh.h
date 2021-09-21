#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "Prerequisites.h"
#include <string>

class Mesh : public Resource
{
	friend class DeviceContext;

public:
	Mesh(const std::string& file_path);
	Mesh(const std::string& name, VertexBufferPtr vertex_buffer, IndexBufferPtr index_buffer);
	
	size_t getIndicesCount() const;
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	
	~Mesh();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
};
